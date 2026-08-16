# Day-Wise GitHub Proof-of-Work Plan

| Day | Focus | Files to Commit | Commit Message | Screenshot/Proof to Capture |
|---|---|---|---|---|
| 1 | Project setup & component planning | `README.md`, folder structure, `.gitignore` | `Initial commit: project structure and README` | Folder tree screenshot |
| 2 | Keypad interfacing | `arduino_code/keypad_module.ino` | `Add 4x4 keypad interfacing module` | Serial Monitor showing masked `*` entry |
| 3 | Password verification | `arduino_code/authentication_module.ino` | `Add password authentication and attempt counter` | Serial log of correct vs wrong attempts |
| 4 | Servo lock control | `arduino_code/lock_control_module.ino` | `Add servo-based lock control with auto-lock timer` | Simulation screenshot of servo unlock/lock |
| 5 | LCD and LED status | LCD wiring added to integrated sketch | `Add LCD status display integration` | LCD showing "Enter Password" / "Access Granted" |
| 6 | Wrong-attempt & buzzer alarm | `arduino_code/alarm_module.ino` | `Add buzzer alarm and lockout logic` | Screenshot of 3rd-fail lockout + buzzer |
| 7 | Complete simulation & testing | `arduino_code/anti_theft_locker_complete.ino`, `docs/testing_strategy.md` | `Integrate complete system and add test results` | Full Wokwi circuit + test log |
| 8 | GitHub documentation | `circuit_diagram/circuit_diagram.md`, final `README.md`, `docs/interview_preparation.md` | `Final README polish and documentation` | GitHub repo homepage preview |

Spreading work across 8 real commits (rather than one large upload) gives an honest, verifiable commit history — exactly what reviewers and interviewers look for in a "proof of work" repo.
