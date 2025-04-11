from typing import Optional, Union, List, Dict, Any, Tuple
import numpy as np
import gymnasium as gym
from gymnasium.vector import VectorEnv, AsyncVectorEnv, SyncVectorEnv
from PendulumDLL.PendulumEnv import PendulumDLLEnv


def make_pendulum_env(env_id: str = "PendulumDLL-v0", **kwargs):
	"""创建单个环境实例"""
	return gym.make(env_id, **kwargs)


def make_vec_pendulum_dll_env(num_envs: int = 4, asynchronous: bool = True, **env_kwargs) -> VectorEnv:
	"""
	创建向量化的PendulumDLL环境

	参数:
	    num_envs: 并行环境的数量
	    asynchronous: 是否使用异步向量环境
	    env_kwargs: 传递给环境构造函数的参数

		PendulumDLL.PendulumEnv:PendulumDLLEnv
	返回:
	    VectorEnv: 向量化的环境实例
	"""

	# 创建多个环境的构造函数
	# 创建多个环境的构造函数
	env_fns = [
		lambda: make_pendulum_env(**env_kwargs)
		for _ in range(num_envs)
	]

	# 创建向量化环境
	if asynchronous:
		return AsyncVectorEnv(env_fns)
	else:
		return SyncVectorEnv(env_fns)
