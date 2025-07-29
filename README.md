# Console Poker Game (Texas Hold’em)

A console-based Texas Hold’em poker game written in C++. Play against AI bots with betting strategies!

## Features

- **Texas Hold’em Rules** – Realistic betting rounds, five community cards, all-in, folding, raising, and pot logic
- **Human vs. Bots** – Play against computer-controlled opponents, including “maniacs” with aggressive strategies
- **Accurate Hand Evaluation** – Determines the winner every round
- **Interactive Console Prompts** – Make choices for every action: bet, call, raise, or fold
- **Clear Action History** – Every player’s decisions are printed out in real time
- **Custom Bot Logic** – AI opponents use different logic for betting and folding (you can customize their style!)

## How to Play

1. **Clone this repo** and compile with any C++17 or later compiler.
2. **Run the game** in your terminal or command prompt in Visual Studio. In VS Code running the program is more complicated.
3. Follow the on-screen instructions to make your moves. Outplay the bots, win the pot, and track your virtual chip stack!

## Getting Started in VS Code

**Compiling:**
```bash
g++ -std=c++17 -o poker *.cpp
./poker
