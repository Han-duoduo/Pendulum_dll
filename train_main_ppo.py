from elegantrl.agents import AgentPPO
from elegantrl import Config
from elegantrl.train import build_env, train_agent, train_agent_single_process, train_agent_multiprocessing  # 单线程训练、多线程训练
from elegantrl import get_gym_env_args
from datetime import datetime
import warnings
from PendulumDLL.PendulumEnv import PendulumDLLEnv
warnings.filterwarnings("ignore")

import gymnasium as gym
from gymnasium.envs.registration import register

register(
    id='PendulumDLL-v0',
    entry_point='PendulumEnv:PendulumDLLEnv',
    kwargs={
        'max_step': 200
    }
)


def train_ppo_for_pendulum():
    agent_class = AgentPPO
    env_class = PendulumDLLEnv
    env_args = {
        'env_name': 'PendulumDLL-v0',
        'state_dim': 3,
        'action_dim': 1,
        'if_discrete': False,
        'num_envs': 1,
        "max_step":200,
    }
    args = Config(agent_class, env_class, env_args)  # see `erl_config.py Arguments()` for hyperparameter explanation
    current_time = datetime.now().strftime("%m-%d %H-%M-%S")
    args.random_seed = 2  # seed
    args.env_seed = None  # 环境种子

    args.cwd = f"./logs/{current_time}-{agent_class.__name__}-{args.env_args['env_name']}-{args.random_seed}"

    args.break_step = int(2e6)  # break training if 'total_step > break_step'
    args.batch_size = 128
    args.horizon_len = 2048
    args.repeat_times = 16  # repeatedly update network using ReplayBuffer to keep critic's loss small
    args.net_dims = [64, 32]  # the middle layer dimension of MultiLayer Perceptron
    args.learning_rate = 6e-5
    args.gamma = 0.97
    args.lambda_entropy = 0.04  # the lambda of the policy entropy term in PPO
    args.clip_grad_norm = 2.0  # 0.1 ~ 4.0, clip the gradient after normalization

    args.num_workers = 4
    args.gpu_id = 0

    # 其他信息
    args.meta_info = {
        "desc": "我是测试消息"
    }

    get_gym_env_args(env=build_env(args.env_class, args.env_args, args.gpu_id), if_print=True)  # return env_args
    # 训练
    train_agent_single_process(args)


if __name__ == '__main__':
    train_ppo_for_pendulum()
