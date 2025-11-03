# 🗓️ FPGenda

**FPGenda** is a simple but effective, lightweight, and fully portable **agenda and task manager** written in **Free Pascal**, using the **fpGUI** toolkit.

Designed for developers and professionals who value clarity and control, FPGenda provides a fast and structured way to **plan, prioritize, and track** your work and personal projects — all in a native desktop environment with no external dependencies.

> 🧠 **Tagline:** “FPGenda — the code of your productivity.”

---

## ✨ Features

- 🧩 Built entirely in **Free Pascal + fpGUI** — cross-platform and dependency-free.
- 📋 Manage **tasks, subtasks, and projects** with priorities, notes, and completion states.
- 🗓️ View tasks by **priority**, **date**, **progress**, or **status**.
- 💾 Save and load agendas from **SQLite** files for easy portability.
- ⚙️ Modular architecture (class include and unit files) separates GUI, logic, and data layers.
- 🧠 Future roadmap:  
  - Calendar and timeline views.  
  - Visual SCRUM/Kanban board.  
  - Sync options and multi-project dashboards.

---

## 🖼️ Screenshot

<img width="782" height="503" alt="image" src="https://github.com/user-attachments/assets/497e2424-fb5b-4656-a4ed-6c3610fb329e" />

---

## 🏗️ Build Instructions

### Requirements
- **Free Pascal Compiler (FPC)** 3.2+  
- **fpGUI Toolkit** (latest build)  
- SQLite libraries (standard with FPC SQLDB package)

### Compile on Linux or Windows

```bash
fpc @extrafpc.cfg FPGenda.pas
```
or build via Lazarus / fpGUI IDE by loading the main program file.

---

🌍 Language Notice

The graphical interface is currently in Portuguese, as the project originated in a Portuguese-speaking environment.
However, most of the source code comments and identifiers are already in English, and a full migration to English UI is planned for future versions.


🧭 Project Philosophy

FPGenda aims to stay:

  - Lightweight — minimal footprint, no heavy frameworks.

  - Cross-platform — consistent behavior in Windows and Linux.

  - Understandable — clear Object Pascal code with educational value.

  - Extendable — modular design encourages community contributions.

---

📜 License

FPGenda is open-source software, released under a permissive Free Software license inspired by the spirit of the GNU GPL but simplified for educational and personal use.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

You are free to use, modify, and distribute this software,
provided that the original authorship is acknowledged and
any derivative works remain equally open for learning and improvement.

---

👥 Authors & Credits

Lead Developer: dnmpt

Language & Framework: Free Pascal / fpGUI

Inspired by: The fpGUI community and the spirit of open desktop software.


🌟 Contribute

Contributions are welcome!

Feel free to fork, open issues, or submit pull requests for:

- Code refactoring and optimization.
- UI/UX improvements.
- Translations (especially for the future English UI).
- Documentation and testing.


💬 Contact & Community

📫 Issues and feedback via GitHub:
https://github.com/dnmpt/FPGenda
