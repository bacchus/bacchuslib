#include "neuro.h"

#include <iostream>

using namespace bacchus;

int main(int argc, char* argv[]) {
    std::cout << "Hello kittie!" << std::endl;

    std::vector<int> topology = {2,3,2};
    Network net(topology);

    float eta = 0.09;
    float alpha = 0.015;

    //while (true) {
    for (int j = 0; j < 1000; ++j) {
        float err = 0.0;
        std::vector< std::vector<float> > inputs = {
            {0,0}, {0,1}, {1,0}, {1,1}
        };
        std::vector< std::vector<float> > outputs = {
            {0,0}, {1,0}, {1,0}, {0,1}
        };
        for (int i = 0; i < inputs.size(); ++i) {
            net.setInput(inputs[i]);
            net.feedForward();
            net.backPropagate(outputs[i], eta,alpha);
            err += net.getError(outputs[i]);
        }

        std::cout << err << std::endl;
        if (err < 0.01) break;
    }

    return 0;
}

