# Keylogger-basic
 
# Process Monitor
![Badge](https://img.shields.io/badge/Issues-1-green)


#


#


  Hello, this is a basic keylogger made using LowLevelKeyboardProc in the future im thinking to update and add soem functions linked to LowLevelMouseProc.

## Installation

1. Clone this repository or download the latest release.
3. Compile and run the application on your Windows machine.

## Usage

1. Launch the project on your Windows machine.
2. The application will continuously monitor the keyboard and it will send the data to the console and it will make .txt files based on the window that has the focus.

## Documentation

We are going to speak more about LowLevekKeyboardProc that is the main function that we use to track the keyboard input. Now let?s take a look at the function:
LRESULT CALLBACK LowLevelKeyboardProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);

We see that there is a **nCode**, Wparam and LParam (as arguments).
**nCode** 
>Indicated whether the hook procedure must process the message.
**wParam** 
>Specifies the type of keyboar dmessage(WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP)

And the last argument which is **lParam**  
>Is a pointer to KBDLLHOOKSTRUCT or in another words is a pointer  to the hookstruct that contains information about keyboard input event.


##Resources
https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644985(v=vs.85)

##Issues
 The main issue tha i got right now (but i will fix it soon) is that the program is supposed to make an txt files that contain the name of the window that has the focus for example `Discord.txt` and store the data there which works a little bit in the beginning but fails after.

## Contributing

Contributions are welcome! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request.
