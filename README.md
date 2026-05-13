# Comparative Analysis of TCP Tahoe, Reno, and Cubic using Reliable Data Transfer over UDP

## Overview

This project implements a reliable data transfer system with end-to-end congestion control using UDP sockets in C language. The project compares three TCP-inspired congestion control algorithms:

- TCP Tahoe
- TCP Reno
- TCP Cubic

Packet transmission probability is used to simulate network congestion and analyze protocol behavior under different network conditions.

---

# Features

- Reliable data transfer using Go-Back-N protocol
- TCP Tahoe congestion control
- TCP Reno congestion control
- TCP Cubic congestion control
- Packet transmission probability simulation
- Congestion window management
- Timeout handling and retransmissions
- CSV logging for analysis
- Graph generation for:
  - Delay
  - Retransmissions
  - Efficiency

---

# Technologies Used

- C Language
- UDP Socket Programming
- Linux / Ubuntu
- Python
- Matplotlib
- Pandas

---

# Project Structure

```text
.
├── sender.c
├── receiver.c
├── graph.py
├── results.csv
├── README.md
├── Project_report.pdf
```

---

# How It Works

## Sender Side

- Accepts dynamic message input
- Sends packets using sliding window protocol
- Maintains congestion window (`cwnd`)
- Implements:
  - TCP Tahoe
  - TCP Reno
  - TCP Cubic
- Detects congestion using:
  - Timeout
  - Duplicate ACKs
- Stores performance metrics in CSV

## Receiver Side

- Receives packets
- Simulates packet loss using transmission probability
- Sends acknowledgments
- Supports reliable packet delivery

---

# Congestion Control Algorithms

## TCP Tahoe
- Resets `cwnd` to 1 after congestion
- Restarts Slow Start

## TCP Reno
- Uses Fast Retransmit and Fast Recovery
- Better recovery than Tahoe

## TCP Cubic
- Faster congestion window growth
- Better performance in high-speed networks

---

# Compilation

```bash
gcc sender.c -o sender
gcc receiver.c -o receiver
```

---

# Execution

## Terminal 1

```bash
./receiver
```

## Terminal 2

```bash
./sender
```

---

# Graph Generation

Install dependencies:

```bash
pip install matplotlib pandas
```

Run graph script:

```bash
python3 graph.py
```

Generated graphs:
- retransmissions.png
- delay.png
- efficiency.png

---

# Experimental Observations

- Retransmissions increase with congestion
- Delay increases due to timeout recovery
- TCP Tahoe shows slower recovery
- TCP Reno improves performance
- TCP Cubic provides better efficiency and throughput

---


# Author

Name: Patil Shradha Sushil
Course: Computer Networks
Academic Year: 2025-26
