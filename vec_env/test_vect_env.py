import gymnasium as gym
import PendulumDLL
import numpy as np
from tqdm import tqdm


def main():
	env_name = "PendulumDLL-v0"
	num_env = 30
	max_steps = 500
	vec_env = PendulumDLL.make_vec_pendulum_dll_env(num_envs=num_env)
	states, _ = vec_env.reset()
	# print(f"initial state: {states}\n")
	# print("*" * 100)
	for i in tqdm(range(max_steps)):
		action = vec_env.action_space.sample()
		next_states, rewards, dones, truncateds, infos = vec_env.step(action)
	# if i % 50 == 0:
	# 	print(
	# 		f"| next states: {next_states}\n"
	# 		f"| rewards: {rewards}\n"
	# 		f"| dones: {dones}\n"
	# 		f"| truncateds: {truncateds}\n"
	# 		f"| steps: {infos['cur_step']}\n"
	# 	)
	vec_env.close()


if __name__ == '__main__':
	main()
