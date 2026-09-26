int classify(int value) {
    if (value > 0) {
        return 1;
    } else {
        return -1;
    }
}

int main(void) {
    return classify(5);
}
