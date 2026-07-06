<div align="center">

![Typing SVG](https://readme-typing-svg.demolab.com/?lines=🎬+Video+Player;C%2B%2B+WinForms+GUI+Project;Data+Structures+%7C+Linked+List;&center=true&width=700&height=80&size=28&weight=700&color=D767F6&pause=1000)

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![WinForms](https://img.shields.io/badge/WinForms-512BD4?style=for-the-badge&logo=dotnet&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linked List](https://img.shields.io/badge/Data_Structure-Linked_List-FF6B6B?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Complete-4CAF50?style=for-the-badge)

</div>

---

## 📌 Overview

A **Windows-based Video Player** built using **C++** and **WinForms GUI** as a **Data Structures project**. The playlist and video queue system is powered by a **Linked List** data structure, demonstrating real-world application of dynamic memory management and node-based data organization. The application provides a clean, user-friendly interface for playing, pausing, and controlling video playback.

> 🎓 Built as a **Data Structures** academic project at **Misr International University (MIU)** — Computer Science Department

---

## 🎬 Demo Video

<div align="center">

[![Watch Demo](https://img.youtube.com/vi/I6ky_biquvg/maxresdefault.jpg)](https://www.youtube.com/watch?v=I6ky_biquvg)

> 👆 Click the thumbnail to watch the full demo on YouTube 🎬

</div>

---

## 🔗 Linked List Implementation

The core of this project uses a **Doubly Linked List** to manage the video playlist:

```
[HEAD] ↔ [Video 1] ↔ [Video 2] ↔ [Video 3] ↔ [Video 4] ↔ [NULL]
```

### Each Node Contains:
| Field | Description |
|-------|-------------|
| `videoPath` | Full file path of the video |
| `videoName` | Display name of the video |
| `duration` | Total duration of the video |
| `*next` | Pointer to the next video node |
| `*prev` | Pointer to the previous video node |

### Linked List Operations:
| Operation | Description |
|-----------|-------------|
| `addVideo()` | Insert new video node at end of playlist |
| `removeVideo()` | Delete a video node from playlist |
| `playNext()` | Move to the next node (next video) |
| `playPrevious()` | Move to the previous node (previous video) |
| `clearPlaylist()` | Delete all nodes and free memory |
| `searchVideo()` | Traverse list to find a video by name |
| `displayPlaylist()` | Traverse and display all video nodes |

---

## ✨ Features

```
🎬 Play / Pause / Stop Controls     ⏩ Fast Forward & Rewind
🔊 Volume Control                   🔇 Mute Toggle
📂 Open & Load Video Files          ⏱️ Progress / Seek Bar
🖥️ Full Screen Mode                 🎨 Clean WinForms GUI
🔗 Linked List Playlist             ⏭️ Next / Previous Video
➕ Add / Remove from Playlist       🔍 Search Video in List
```

---

## 🖥️ GUI Components

| Component | Description |
|-----------|-------------|
| 🎬 **Video Panel** | Main display area for video rendering |
| ▶️ **Play / Pause Button** | Toggle video playback |
| ⏹️ **Stop Button** | Stop and reset video |
| ⏩ **Forward / Rewind** | Skip forward or backward |
| ⏭️ **Next / Previous** | Navigate linked list nodes |
| 🔊 **Volume Slider** | Adjust audio level |
| 🔇 **Mute Button** | Toggle sound on/off |
| 📊 **Progress Bar** | Shows and controls playback position |
| 📂 **Open File Button** | Browse and load video files |
| 📋 **Playlist Panel** | Displays all videos in the linked list |
| 🖥️ **Fullscreen Button** | Toggle fullscreen mode |

---

## 🛠️ Tech Stack

| Technology | Purpose |
|-----------|---------|
| ⚙️ C++ | Core application logic |
| 🔗 Linked List | Playlist and video queue management |
| 🎨 WinForms | GUI design and event handling |
| 🎬 Windows Media Player Control | Video rendering and playback |
| 📁 File Dialog API | Browse and open video files |
| 🔊 Audio API | Volume and sound control |

---

## 📂 Project Structure

```
Video-Player/
│
├── 📁 src/
│   ├── 📄 Main.cpp                     # Entry point of the application
│   ├── 📄 VideoPlayer.cpp              # Core video player logic
│   ├── 📄 VideoPlayer.h                # Header file for player class
│   │
│   ├── 📁 DataStructures/
│   │   ├── 📄 Node.h                   # Video node structure (data + pointers)
│   │   ├── 📄 LinkedList.cpp           # Linked list implementation
│   │   └── 📄 LinkedList.h             # Linked list header file
│   │
│   ├── 📁 GUI/
│   │   ├── 📄 MainForm.cpp             # Main window form logic
│   │   ├── 📄 MainForm.h               # Main form header
│   │   ├── 📄 MainForm.Designer.cpp    # Auto-generated WinForms layout
│   │   └── 📄 ControlPanel.cpp         # Playback control buttons panel
│   │
│   ├── 📁 Controls/
│   │   ├── 📄 PlaybackController.cpp   # Play, pause, stop logic
│   │   ├── 📄 VolumeController.cpp     # Volume and mute control
│   │   ├── 📄 ProgressBar.cpp          # Seek bar and progress tracking
│   │   └── 📄 FileManager.cpp          # File open and loading logic
│   │
│   └── 📁 Utils/
│       ├── 📄 TimeFormatter.cpp        # Formats time display (00:00)
│       └── 📄 FullscreenManager.cpp    # Handles fullscreen toggle
│
├── 📁 resources/
│   ├── 📁 icons/                       # Button icons and app icon
│   └── 📁 assets/                      # UI assets and images
│
├── 📄 VideoPlayer.sln                  # Visual Studio solution file
├── 📄 VideoPlayer.vcxproj              # Visual Studio project file
└── 📄 README.md
```

---

## 🚀 How to Run

### Requirements
- **Windows OS** (Windows 10 or higher recommended)
- **Visual Studio 2019 or 2022** with C++/CLI support
- **.NET Framework** (for WinForms)
- **Windows Media Player** installed on your system

### Steps

**1. Clone the repository:**
```bash
git clone https://github.com/shahd-w21/Video-Player.git
cd Video-Player
```

**2. Open in Visual Studio:**
- Double-click `VideoPlayer.sln`
- Visual Studio will load the full project

**3. Build the project:**
- Press **Ctrl + Shift + B** to build
- Or go to **Build → Build Solution**

**4. Run the application:**
- Press **F5** to run with debugger
- Or **Ctrl + F5** to run without debugger

---

## 🎮 How to Use

| Step | Action |
|------|--------|
| 1 | 📂 Click **Open File** to browse and load a video |
| 2 | ▶️ Press **Play** to start playback |
| 3 | ⏸️ Press **Pause** to pause at any point |
| 4 | ⏩ Use **Forward / Rewind** to skip through the video |
| 5 | ⏭️ Press **Next / Previous** to navigate the playlist |
| 6 | ➕ Click **Add to Playlist** to insert a new video node |
| 7 | 🔊 Drag the **Volume Slider** to adjust audio |
| 8 | 📊 Click anywhere on the **Progress Bar** to seek |
| 9 | 🖥️ Press **Fullscreen** for an immersive experience |

---

## 🎞️ Supported Formats

| Format | Supported |
|--------|-----------|
| `.mp4` | ✅ |
| `.avi` | ✅ |
| `.mkv` | ✅ |
| `.wmv` | ✅ |
| `.mov` | ✅ |
| `.flv` | ✅ |

---
*"Because every great project deserves a great player."* 🎬

![Visitor Badge](https://visitor-badge.laobi.icu/badge?page_id=shahd-w21.Video-Player)

</div>
