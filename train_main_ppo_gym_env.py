from elegantrl.agents import AgentPPO
from elegantrl import Config
from elegantrl.train import build_env, train_agent, train_agent_single_process, train_agent_multiprocessing  # 单线程训练、多线程训练
from elegantrl import get_gym_env_args
from datetime import datetime
import warnings

warnings.filterwarnings("ignore")

import gymnasium as gym

def train_ppo_for_pendulum():
    agent_class = AgentPPO
    env_class = gym.make
    env_args = {
        'env_name': 'Pendulum-v1',
        'state_dim': 3,
        'action_dim': 1,
        'if_discrete': False,
        'num_envs': 1,
        "max_step": None,
    }
    args = Config(agent_class, env_class, env_args)  # see `erl_config.py Arguments()` for hyperparameter explanation
    current_time = datetime.now().strftime("%m-%d %H-%M-%S")
    args.random_seed = 45  # seed
    args.env_seed = 1  # 环境种子

    args.cwd = f"./logs/{current_time}-{agent_class.__name__}-{args.env_args['env_name']}-{args.random_seed}"

    args.break_step = int(1e6)  # break training if 'total_step > break_step'
    args.batch_size = 128
    args.horizon_len = 2048
    args.repeat_times = 16  # repeatedly update network using ReplayBuffer to keep critic's loss small
    args.net_dims = [64, 32]  # the middle layer dimension of MultiLayer Perceptron
    args.learning_rate = 6e-5
    args.gamma = 0.97
    args.lambda_entropy = 0.04  # the lambda of the policy entropy term in PPO

    args.num_workers = 10
    args.gpu_id = 0

    # 其他信息
    args.meta_info = {
        "desc": "多线程训练"
    }

    get_gym_env_args(env=build_env(args.env_class, args.env_args, args.gpu_id), if_print=True)  # return env_args
    # 训练
    train_agent_single_process(args)


if __name__ == '__main__':
    train_ppo_for_pendulum()
