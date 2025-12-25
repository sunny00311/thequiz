# 🎯 KBC Quiz Game in C (Terminal-Based)

A feature-rich **Kaun Banega Crorepati (KBC)** style quiz game built in **C**, designed to run in the terminal.  
This project demonstrates core C programming concepts such as file handling, structures, timers, lifelines, and API integration—without using heavy external libraries.

---

## 🚀 Features

- 📄 **Dynamic Question Loading**  
  Reads questions from a text file (`questions.txt`)

- ⏱️ **Timed Questions**  
  Each question has a time limit; unanswered questions result in a timeout

- 🎨 **Colored Terminal Interface**  
  Uses ANSI escape codes for better readability

- 🧠 **Lifelines**
  - **50/50** – Removes two wrong options
  - **Skip Question** – Skip without penalty
  - **Ask the Expert** – Calls an external API for help

- 🌐 **API Integration**
  - Expert advice fetched via `curl` using `system()`  
  - API Endpoint: `https://npp-mauve.vercel.app/ask`

- 🔀 **Randomized Questions**
  - Questions are shuffled on every run

- 💰 **Prize Money System**
  - Each question carries a reward value

---

## 🛠️ Tech Stack

- **Language:** C
- **Compiler:** GCC / MinGW
- **Platform:** Windows
- **Libraries Used:**
  - `stdio.h`
  - `stdlib.h`
  - `string.h`
  - `ctype.h`
  - `time.h`
  - `conio.h`

---

## 📂 Project Structure

