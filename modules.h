void neuron(float X, float Y) {
    for(int f = 0; f < 20; f++) {
        float theta = 2.0 * 3.1415926 * float(f) / float(20);

        float x = 0.06 * std::cos(theta);
        float y = 0.06 * std::sin(theta);

        glVertex2f(x - X, y + Y);
    }
}

int innovation_number(int a, int b) {
   int times = 1;
   while (times <= b)
      times *= 10;
   return a*times + b;
} 

float random_position() {
    srand(time(0));
    return -20 + rand() % (( 61 + 1 ) - 20);
}

int random_connection(int size) {
    return 0 + rand() % (( size + 1 ) - 0);
}

int random_weight() {
    return std::rand() % 10 + (-1);
}

int random_input() {
    return 1 + rand() % (( 8 + 1 ) - 1);
}

int random_output() {
    return 9 + rand() % (( 10 + 1 ) - 9);
}