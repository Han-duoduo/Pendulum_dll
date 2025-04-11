from gymnasium.envs.registration import register
from PendulumDLL.PendulumEnv import PendulumDLLEnv
from PendulumDLL.vector_env import make_vec_pendulum_dll_env

# 注册环境
register(
	id="PendulumDLL-v0",
	entry_point="PendulumDLL.PendulumEnv:PendulumDLLEnv",  # 目录.py文件:类名
	kwargs={
		'max_step': 200
	}
)

# 暴露类和函数
__all__ = ["PendulumDLLEnv", "make_vec_pendulum_dll_env"]
