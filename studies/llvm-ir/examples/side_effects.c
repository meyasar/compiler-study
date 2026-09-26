extern void log_value(int value);

int analyze_effects(int input, int *output) {
    int unused = input * 2;
    *output = input;

    log_value(input);

    return input + 1;
}
