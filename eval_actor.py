import gymnasium as gym
import numpy as np
from elegantrl.train import build_env
from elegantrl import Config
import torch as th
from gymnasium.envs.registration import register

register(
    id='PendulumDLL-v0',
    entry_point='PendulumEnv:PendulumDLLEnv',
    kwargs={
        'max_step': 200
    }
)


def eval_policy(pt_file: str, args: Config):
    # 创建评估环境
    # eval_env = build_env(args.env_class, args.env_args)
    eval_env = gym.make(id=args.env_args['env_name'])

    # 加载actor网络
    actor = th.load(pt_file, map_location=lambda storage, loc: storage, weights_only=False)
    # actor.load_state_dict(
    #     th.load(pt_file, map_location=lambda storage, loc: storage, weights_only=False)
    # )  # 加载actor，保留权重默认设备
    # 进行评估
    avg_reward = 0.0
    for i in range(args.eval_times):
        # 初始化环境
        state, _ = eval_env.reset(seed=args.env_seed)
        rewards = 0.0
        step = 0
        thetas = [45.0]
        while True:
            action = actor(th.as_tensor(state, dtype=th.float32).unsqueeze(0)).cpu().detach().numpy()
            print(f"action: {action}")
            state, reward, terminated, truncated, info = eval_env.step(action)
            rewards += reward
            step += 1
            if terminated or truncated:
                break
            thetas.append(np.rad2deg(info['theta']))
            print(f"当前角度：{np.rad2deg(info['theta'])}")
        avg_reward += rewards
        print(
            f"eval: ===========[{i + 1}/{args.eval_times}]===========\n"
            f"rewards: {rewards:.3f}\n"
            f"step: {step}"
        )
        # 绘图plot
        import matplotlib.pyplot as plt
        plt.figure(dpi=150, figsize=(6, 4))
        plt.plot(thetas)
        plt.title(f"rewards:{avg_reward:.3f}")
        plt.xlabel("step/(one step means 0.05s)")
        plt.ylabel("angle(°)")
        plt.grid(True)
        plt.savefig(f"{i}_Reward={avg_reward:.3f}.png")
        # plt.show()

        print("*" * 100)
    avg_reward /= args.eval_times
    print(
        f"======== 【avg_reward】: {avg_reward:.3f} ============"
    )


if __name__ == '__main__':
    env_args = {
        'env_name': 'PendulumDLL-v0',
        'state_dim': 3,
        'action_dim': 1,
        'if_discrete': False,
        'num_envs': 1,
        'max_step':200
        # 'render_mode': 'human'
    }
    # config参数
    args = Config(None, None, env_args)
    args.eval_times = 1  # 设置评估次数
    args.env_seed = 0  # 环境种子

    # 加载actor路径
    actor_path = r"./logs/03-23 12-09-09-AgentTD3-PendulumDLL-v0-26/actor__000000319488.pt"

    # 进行评估
    eval_policy(actor_path, args)
