import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("results.csv")

algo_map = {
    1: "Tahoe",
    2: "Reno",
    3: "Cubic"
}

data["algorithm"] = data["algorithm"].map(algo_map)

data = data.sort_values(by="probability")

# Graph 1: Retransmissions

plt.figure(figsize=(8,5))

for algo in data["algorithm"].unique():

    subset = data[data["algorithm"] == algo]

    plt.plot(
        subset["probability"],
        subset["retransmissions"],
        marker='o',
        label=algo
    )

plt.xlabel("Packet Transmission Probability")
plt.ylabel("Retransmissions")
plt.title("Retransmissions vs Transmission Probability")
plt.grid(True)
plt.legend()

plt.savefig("retransmissions.png")
plt.show()

# Graph 2: Delay

plt.figure(figsize=(8,5))

for algo in data["algorithm"].unique():

    subset = data[data["algorithm"] == algo]

    plt.plot(
        subset["probability"],
        subset["time"],
        marker='o',
        label=algo
    )

plt.xlabel("Packet Transmission Probability")
plt.ylabel("Time Taken (sec)")
plt.title("Delay vs Transmission Probability")
plt.grid(True)
plt.legend()

plt.savefig("delay.png")
plt.show()

# Graph 3: Efficiency

plt.figure(figsize=(8,5))

for algo in data["algorithm"].unique():

    subset = data[data["algorithm"] == algo]

    plt.plot(
        subset["probability"],
        subset["efficiency"],
        marker='o',
        label=algo
    )

plt.xlabel("Packet Transmission Probability")
plt.ylabel("Efficiency (%)")
plt.title("Efficiency vs Transmission Probability")
plt.grid(True)
plt.legend()

plt.savefig("efficiency.png")
plt.show()

print("\nGraphs generated successfully!")