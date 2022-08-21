import argparse
import random

INT_MAX = 2 ** 31 - 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--file", "-f", type=str, default="inputs/io-test.txt", help="Output file"
    )
    parser.add_argument("--max-n", "-n", type=int, default=10000, help="Max N")
    parser.add_argument(
        "--insert-ops",
        "-i",
        type=int,
        default=10000,
        help="Number of Insert operations",
    )
    parser.add_argument(
        "--remove-ops",
        "-r",
        type=int,
        default=10000,
        help="Number of Remove operations",
    )
    parser.add_argument(
        "--description", "-d", type=str, default="Test data", help="Description of the test"
    )
    parser.add_argument("--seed", "-s", type=int, default=42, help="Random Seed")
    args = parser.parse_args()

    assert args.insert_ops <= args.max_n
    assert args.remove_ops <= args.insert_ops

    # prepare structures
    random.seed(args.seed)
    hashmap_state = []
    insert_pool = list(range(args.max_n))
    file = open(args.file, "w")
    # add headers
    file.write(f"# {args.description}\n")
    file.write(f"n {args.max_n}\n")

    # generate insertions
    file.write(f"# Inserting {args.insert_ops}\n")
    random.shuffle(insert_pool)
    for i in range(args.insert_ops):
        n = insert_pool.pop(0)
        file.write(f"insert {n} {random.randint(0, INT_MAX)}\n")
        hashmap_state.append(n)

    # generate removals
    file.write(f"# Removing {args.remove_ops}\n")
    random.shuffle(hashmap_state)
    for i in range(args.remove_ops):
        n = hashmap_state.pop(0)
        file.write(f"remove {n}\n")

    file.close()
    print("Done")
