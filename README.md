# Pokemon-Battle-Simulation
Game for two users to battle using pokémon

## Backstory
Mini project developed for the **Intro to Programming in C** course during 1<sup>st</sup> semester, 2017. 

The challenge was to use basic features of C and only standard library.

## Features
+ Already coded with 50+ *pokémons* and 70+ *moves*
+ Can add new *moves* and *pokémons*

## Development
+ Developed [Moves](Moves) and [Pokemon](Pokemon) projects using **CodeBlocks** on **Windows 7**.
+ Developed [Final](Final) project using **CodeBlocks** on **Windows 8**.
+ Testen on **Windows 7**, **Windows 8** and **Windows 10**.

## Details about files/folders
+ The files **move_details.dat** and **pokemon_details.dat** contain the *moves* and *pokémons* details respectively. All the 
project/source code accesses/uses these files.
+ The folder [Moves](Moves) contain the CodeBlocks project file and source code to modify the **move_details.dat** file.
+ The folder [Pokemon](Pokemon) contain the CodeBlocks project file and source code to read **move_details.dat** file and modify 
**pokemon_details.dat** file.
+ The folder [Final](Final) contain the ***main game*** CodeBlocks project file and source code. This project needs to read both 
**move_details.dat** and **pokemon_details.dat** files.

## Installing/Cloning
+ To clone it on your local machine,
  ```shell
  git clone https://github.com/OjeshManandhar/Pokemon-Battle-Simulation.git
  ```
+ To make changes to *moves*
  - Open [Moves](Moves) folder
  - Run *Moves.cbp* using CodeBlocks or open *main.c*
  - Compile and Run
+ To make changes to *pokémons*
  - Open [Pokemon](Pokemon) folder
  - Run *Pokemon.cbp* using CodeBlocks or open *main.c*
  - Compile and Run
+ To play the game
  - Open [Final](Final) folder
  - Run *Final.cbp* using CodeBlocks or open *main.c*
  - Compile and Run
  
## Further Improvement
If you're interested, feel free to fork and submit PR.
- [ ] Add timer to limit battle time
- [ ] New battle modes
- [ ] Add feature to make computer battle
- [ ] Use interactive graphics
- [ ] Add pokémon images
- [ ] Add status effects to pokémons during battle
- [ ] Adding features to allow user to use more than one pokémons during a battle
- [ ] Saving unfinished battels for continuing next time
- [ ] Add new battle modes

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Authors
[Ojesh Manandhar](https://github.com/OjeshManandhar): Coded the file handling part

[Sanil Manandhar](https://github.com/Sanilblank): Data Collection, Brainstorming guy

[Samip Shrestha](https://github.com/samip584): Coded the battle simulation part

[Shrijan Bajracharya](https://github.com/ShriBuzz): Data Collection, Brainstorming guy
