# AI Game Controller

A modern C++ artificial intelligence project that combines a custom SFML platforming simulation with NeuroEvolution of Augmenting Topologies (NEAT).

AI Game Controller recreates a level inspired by Yoshi's Island 1 from Super Mario World and uses evolutionary computation to train an artificial neural network capable of controlling Mario without scripted gameplay behaviour. Rather than relying on predefined rules or decision trees, the controller learns through repeated evaluation, fitness optimisation and genetic evolution.

Developed as the practical component of my undergraduate dissertation, the project demonstrates how machine learning, game simulation and real-time C++ systems can be combined to investigate autonomous gameplay.

---

## The Engineering Problem

Most videogame AI relies on predefined rules, finite-state machines or decision trees. While these approaches can produce predictable behaviour, they cannot independently improve through experience.

AI Game Controller explores an alternative approach by combining a complete platforming simulation with NeuroEvolution of Augmenting Topologies (NEAT). The challenge was to evolve an artificial neural network capable of navigating a complex level using only information gathered from the surrounding environment, while maintaining deterministic gameplay, repeatable evaluation and measurable fitness across successive generations.

---

## AI System Architecture

<p align="center">
    <img width="600" alt="AIGameController" src="https://github.com/user-attachments/assets/171ed8f7-6974-40f0-91b9-648113515876" />
</p>

The project is organised into distinct layers that separate the platforming simulation, neural controller and evolutionary training systems. Gameplay provides a deterministic environment for evaluation, while the controller interprets environmental information and NEAT evolves increasingly capable neural networks over successive generations.

---

## Engineering Goals

The project was designed to demonstrate:

- NeuroEvolution of Augmenting Topologies (NEAT)
- Artificial neural networks
- Evolutionary computation
- Genetic algorithms
- Real-time game simulation
- Modular gameplay architecture
- AI visualisation and debugging

Although demonstrated through autonomous gameplay, the engineering principles apply equally to robotics, autonomous systems, optimisation problems, simulation software and intelligent control systems.

---

## Key Features

- Complete 2D platforming simulation built with SFML
- Autonomous gameplay using evolved neural networks
- NeuroEvolution of Augmenting Topologies (NEAT)
- Configurable environmental input representations
- Fitness-based evolutionary training
- Neural-network visualisation and debugging tools
- Human and AI controllers operating within the same simulation

---

## Technologies & Engineering Practices

### Development

- Modern C++
- SFML
- Visual Studio
- Git

### Engineering Practices

- Artificial Neural Networks
- NeuroEvolution of Augmenting Topologies
- Genetic Algorithms
- Evolutionary Computation
- Real-Time Simulation
- Object-Oriented Design
- State-Driven Architecture
- Collision Detection
- Separation of Concerns

---

## AI Architecture Highlights

- Modular separation between gameplay and AI systems
- Shared player implementation for human and AI control
- Grid-based environmental sensing
- Fitness-driven evolutionary learning
- Species management and topology evolution
- Neural-network visualisation tools

---

## Engineering Outcome

AI Game Controller demonstrates how evolutionary computation can be integrated into a real-time C++ application to produce autonomous gameplay.

By separating the platforming simulation from the neural controller and evolutionary systems, the project provides a repeatable environment in which artificial neural networks can be trained and evaluated. The resulting controller successfully learns to navigate the level through fitness-driven evolution, demonstrating the practical application of machine learning, simulation and software engineering within an interactive system.

---
