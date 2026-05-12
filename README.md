# AI Game Controller

A modular **C++ / SFML game AI project** demonstrating an AI-controlled agent for a 2D platformer. The project explores neuroevolution, autonomous gameplay, fitness evaluation, simulation loops, and reusable game engine systems.

The system uses an evolutionary learning approach to train neural-network-based controllers to navigate a platformer environment and compare AI behaviour against manual human play.

---

## 🎥 Preview

| AI-Controlled Mode | Manual Mode |
|--------------------|-------------|
| ![AI Demo](https://raw.githubusercontent.com/MoAgilah/AI-Game-Controller/master/SFMLGameEngine/Previews/GIFs/AutomatedDemo.gif) | ![Manual Demo](https://raw.githubusercontent.com/MoAgilah/AI-Game-Controller/master/SFMLGameEngine/Previews/GIFs/ManualDemo.gif) |

---

## 🧠 Overview

This project is a refactored version of a Master’s dissertation exploring how an AI agent can learn to play a 2D platformer level using evolutionary learning techniques.

The project combines a custom 2D game framework with an AI controller, allowing automated agents to be evaluated, selected, and improved across generations using gameplay performance metrics.

---

## ⚙️ Key Features

- AI controller using evolutionary learning strategies
- Neural-network-based gameplay control
- Fitness evaluation based on gameplay performance
- Manual play mode for testing and comparison
- Custom 2D engine systems built with C++ and SFML
- Game state management for menus, loading, gameplay, and pause states
- Physics and collision handling
- Player and enemy entity systems
- Debugging and simulation tooling
- Performance logging for AI experimentation

---

## 🤖 AI System

The AI controller uses a population-based learning process where each controller attempts to complete the level and is scored using a fitness function.

### Core Process

1. Generate a population of neural-network controllers.
2. Run each controller through the platformer level.
3. Evaluate performance using fitness metrics such as distance, survival, and completion.
4. Select stronger controllers for mutation and crossover.
5. Repeat across generations to improve gameplay behaviour.

This allows the agent to gradually learn movement, jumping, obstacle avoidance, and level progression.

---

## 🧱 Architecture

The project is structured around reusable game and AI systems. Gameplay, AI decision-making, physics, rendering, and state management are kept modular to support maintainability and experimentation.

```text
AI-Game-Controller/
└── SFMLGameEngine/
    ├── Bin/          → Downloadable builds
    ├── Code/         → Source code
    ├── Resources/    → Fonts, sprites, shaders, and outputs
    ├── Previews/     → Screenshots and GIFs
    └── README.md     → Project documentation
```

---

## 🛠 Technologies

- C++
- SFML
- Visual Studio
- Object-oriented programming
- Evolutionary algorithms
- Artificial neural networks
- 2D physics and collision systems
- Game state architecture

---

## 🚀 Build & Run

### Requirements

- Windows 10 or later
- Visual Studio 2019 or 2022
- SFML-compatible environment

### Steps

1. Clone the repository.
2. Open the Visual Studio solution.
3. Build the project.
4. Run the executable.

A standalone executable is also available in the repository download section.

---

## 🔭 Future Work

- Save/load support for trained neural networks
- Visual AI debugging overlays
- Additional level files and training scenarios
- Adjustable AI training parameters
- Cross-platform release builds
- Improved simulation speed and optimisation

---
