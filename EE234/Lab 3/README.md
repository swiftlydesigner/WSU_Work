# Lab 3

All files created by Kyle Parker
    EE234 at Washington State University Fall 2022

## Reqs

Use assembly to make a stopwatch using the Blackboard. ARM9 Assembly 
(see [realdigital.org] for details).

### Stopwatch.s

Make a stopwatch.
- Initialize your stopwatch to 00.00 (s). Set the state to ST_RESET.
- The following describes what to do for each push button input (rising edge).
- State [ST_RESET]
    - BTN0: Start the stopwatch. Next state [ST_RUN]
    - BTN3: Nothing happens. Next state [ST_RESET]
- State [ST_RUN]
    - BTN0: Stop the stopwatch. Next state [ST_STOP]
    - BTN3: Stop visualizing the stopwatch, but keep counting in the background. Next state [ST_BG]
- State [ST_STOP]
    - BTN0: Resume the stopwatch. Next state [ST_RUN]
    - BTN3: Stop the stopwatch and initialize your stopwatch. Next state [ST_RESET]
- State [ST_BG]
    - BTN0: Stop counting. Show the last time (that was being counted in the background) on the seven-segment display. Next state [ST_STOP]
    - BTN3: Keep counting and resume visualizing the stopwatch. Next state [ST_RUN]
