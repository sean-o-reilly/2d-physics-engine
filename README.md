# 2d-physics-engine
A simple (in progress) physics engine built with raylib and C++, which I started over winter break. 

I've recently returned to this project to refactor it and scale it into a fully fledged simulator. I'm looking to set up a stronger foundation for the project, while taking things slower and thoughtfully planning out implementations. 

The goal currently is to set up the application around "Environments", which will serve as the flagship class for the engine. Environments will manage physics interactions, viewports, draw calls, serialization, etc., which will help the project avoid design pitfalls and scale better over time. 
