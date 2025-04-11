import gymnasium as gym
from gymnasium import spaces
import numpy as np
import ctypes
from ctypes import wintypes
import shutil
import os
import tempfile
import warnings
from typing import Tuple

from tqdm import tqdm

warnings.filterwarnings("ignore")
ARRAY = np.ndarray

#  打包的dll环境
# DLL_ENV_PATH = r"rlSimplePendulumModel_win64.dll"
DLL_ENV_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "rlSimplePendulumModel_win64.dll")

kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
kernel32.FreeLibrary.argtypes = [wintypes.HMODULE]
kernel32.FreeLibrary.restype = wintypes.BOOL


## 输入、输出结构体
class ExtU_rlSimplePendulumModel_T(ctypes.Structure):
	_fields_ = [
		("action", ctypes.c_double),
	]


class ExtY_rlSimplePendulumModel_T(ctypes.Structure):
	_fields_ = [
		("theta", ctypes.c_double),
		("theta_dot", ctypes.c_double),
	]


# 环境封装
class PendulumDLLEnv(gym.Env):
	def __init__(self, max_step=200):
		self.ts = 0.05  # 仿真步长(s)
		self.max_step: int = max_step  # 最大步数
		self.tf = self.max_step * self.ts  # 仿真时长(s)

		# 输入输出维度
		self.discrete: bool = False  # 连续动作
		self.action_dim: int = 1
		self.action_space = spaces.Box(low=-1, high=1, shape=(self.action_dim,), dtype=np.float32)
		self.state_dim: int = 3
		self.observation_space = spaces.Box(
			low=-np.array([1.0, 1.0, 8.0]), high=np.array([1.0, 1.0, 8.0]), shape=(self.state_dim,), dtype=np.float32
		)

		# 定义动作缩放
		self.action_scale = 2.0  # 动作放缩
		# 奖励值缩放
		self.reward_scale = 0.5  # 奖励放缩

		# 初始化当前步数
		self._cur_step = None
		# 加载dll
		self._load_dll()

	## 加载dll
	def _load_dll(self):
		# 创建一个dll副本
		self._env_dll_handler = tempfile.mkdtemp()
		self._temp_dll_path = os.path.join(self._env_dll_handler, os.path.basename(DLL_ENV_PATH))
		# 确保目录存在
		os.makedirs(os.path.dirname(self._temp_dll_path), exist_ok=True)
		shutil.copyfile(DLL_ENV_PATH, self._temp_dll_path)
		# 加载临时副本
		self._env_dll = ctypes.WinDLL(self._temp_dll_path)

		if not self._env_dll:
			raise RuntimeError("Load dll: failed!")
		action_ptr = self._env_dll.rlSimplePendulumModel_U
		state_ptr = self._env_dll.rlSimplePendulumModel_Y
		# 动作输入
		self._action_structure = ctypes.cast(
			action_ptr, ctypes.POINTER(ExtU_rlSimplePendulumModel_T)
		).contents
		# 状态输出
		self._state_structure = ctypes.cast(
			state_ptr, ctypes.POINTER(ExtY_rlSimplePendulumModel_T)
		).contents

	def close(self):  # 关闭环境
		self._unload_dll()

	## 卸载dll
	def _unload_dll(self):
		try:
			if self._env_dll:
				# 先释放DLL句柄
				kernel32.FreeLibrary(self._env_dll._handle)
				self._env_dll = None

			if os.path.exists(self._env_dll_handler):
				# 添加重试机制和错误忽略
				shutil.rmtree(self._env_dll_handler, ignore_errors=True)

		except Exception as e:
			print(f"清理临时文件时发生错误: {str(e)}")

	## 重置环境
	def reset(self, **kwargs):
		self.close()  # 卸载dll
		self._load_dll()  # 重新加载dll
		# 初始化环境
		self._cur_step = 0
		self._env_dll.rlSimplePendulumModel_initialize()
		self._env_dll.rlSimplePendulumModel_step()
		self._action_structure.action = 0.0  # 初始动作
		info = {
			'cur_step': self._cur_step,
			'theta': float(self._state_structure.theta),
			'theta_dot': float(self._state_structure.theta_dot),
		}
		return self._get_obs(), info

	## 步进环境
	def step(self, action) -> Tuple[ARRAY, float, bool, bool, dict]:
		# action = np.clip(action, -1, 1)
		if self._cur_step == None:
			raise RuntimeError("Please reset the environment with 'reset()' method. ")
		if self._cur_step >= self.max_step:
			raise RuntimeError("The maximum number of steps has been reached!,please use 'reset()' method. ")
		# 实际执行动作 = action * action_scale
		self._action_structure.action = action * self.action_scale
		self._env_dll.rlSimplePendulumModel_step()  # 执行一次
		# 下一刻状态
		next_state: ARRAY = self._get_obs()
		# 当前步数+1
		self._cur_step += 1
		# 游戏是否结束
		terminated = False
		# 最大到达步长
		truncated = self._cur_step >= self.max_step
		# 计算奖励值
		reward = float(self._calculate_reward(action))
		# 其他信息
		info = {
			'cur_step': self._cur_step,
			'theta': float(self._state_structure.theta),
			'theta_dot': float(self._state_structure.theta_dot),
		}

		return next_state, reward, terminated, truncated, info

	## 获取状态
	def _get_obs(self) -> ARRAY:
		theta = float(self._state_structure.theta)
		theta_dot = float(self._state_structure.theta_dot)
		obs = np.array([np.cos(theta), np.sin(theta), theta_dot], dtype=np.float32)
		return obs

	def _calculate_reward(self, action):
		theta = float(self._state_structure.theta)
		theta_dot = float(self._state_structure.theta_dot)
		return -(theta ** 2 + 0.1 * theta_dot ** 2 + 0.001 * action ** 2) * self.reward_scale


if __name__ == "__main__":
	env = PendulumDLLEnv()
	env.reset()
	repeat_time = 3
	for i in range(repeat_time):
		print(f"==========正在进行第{i + 1}次环境测试==========")
		env.reset()
		thetas = [0.785]
		theta_dot = [0.0]
		t = 0
		sin_wave = [np.sin(t)]
		for step in tqdm(range(env.max_step)):
			action = sin_wave[-1]
			state, reward, terminated, truncated, info = env.step(1)
			t += env.ts
			sin_wave.append(np.sin(t))
			thetas.append(info['theta'])
			theta_dot.append(info['theta_dot'])
			# print(
			#     f"action: {action}\n"
			#     f"info: {info}\n"
			# )
			if terminated or truncated:
				# print(f"terminated: {terminated}\n" f"truncated: {truncated}\n")
				break
		# 绘图
		import matplotlib.pyplot as plt

		fig, axs = plt.subplots(2, 1, dpi=150, figsize=(6, 10))
		ax1 = axs.flat[0]
		ax2 = axs.flat[1]
		# 图1
		ax1.plot(thetas, label="theta")
		ax1.plot(theta_dot, label="theta_dot")
		ax1.legend()
		ax1.set_title(f"Pendulum-{i + 1}")
		ax1.set_xlim([0, env.max_step])
		ax1.grid(True)

		# 图2
		ax2.plot(sin_wave)
		ax2.grid(True)
		ax2.set_title("action")
		ax2.set_xlim([0, env.max_step])

		plt.tight_layout(pad=1.5)
		plt.show()
	env.close()
