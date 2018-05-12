# cpp_hopfield_digit_recognition
Code written in 2008  for neural network college classes.

Old code updated to new visual studio and wxWidget version 3.1.1

![Alt text](/screen.png?raw=true "Screen")

Legend:
1. Learning data presentation
2. Validation data (test with known result)
    6-6(100%) - first digit is expected output, second digit is neural network result(percent is neural network confidence to this result)
                when you expand combobox, you can see other result with less percentage
3. Test result summary :
      Error 100% - neural network return 100% for invalid output
      Error      - neural network return invalid output, but confidence was less then 100%
      Ok 100%    - neural network valid guess
4. Valid result position in neural network guess 
      first position - valid guess on 1 position
      10th position  - valid guess was on last position (very bad result)
5. Test data confidence distribution
6. Test digit - network result in combobox
