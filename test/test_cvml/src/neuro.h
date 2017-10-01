#pragma once

#include "math/mdefs.h"

#include <vector>

namespace bacchus {

class Neuron;

struct Connection {
    Connection(Neuron* neuron)
        : connectedNeuron(neuron)
        , weight(rnd())         // normal distrib
        , dweight(0.0)
    {}

    Neuron* connectedNeuron;
    float weight;
    float dweight;
};

class Neuron {
public:
    Neuron(const std::vector<Neuron*>& layer) {
        for (Neuron* neuron: layer) {
            dendrons.push_back(new Connection(neuron));
        }
    }

    static float sigmoid(float x) { return 1.0/(1.0 + exp(-x)); }
    static float dsigmoid(float x) { return x*(1.0 - x); }
    void setError(float err) { error = err; }
    void addError(float err) { error += err; }
    void setOutput(float out) { output = out; }
    float getOutput() const { return output; }

    void feedForward() {
        float sumOut = 0.0;
        if (dendrons.size() > 0) {
            for (Connection* dendron: dendrons) {
                sumOut += dendron->weight * dendron->connectedNeuron->getOutput();
            }
            output = sigmoid(sumOut);
        }
    }

    void backPropagate(float eta, float alpha) {
        grad = error * dsigmoid(output);
        for (Connection* dendron: dendrons) {
            Neuron* neuron = dendron->connectedNeuron;
            dendron->dweight = eta*neuron->output*grad + alpha*dendron->dweight;
            dendron->weight += dendron->dweight;
            neuron->addError(dendron->weight*grad);
        }
        error = 0.0;
    }

private:
    std::vector<Connection*> dendrons;
    float error = 0.0;
    float grad = 0.0;
    float output = 0.0;
};

class Network {
public:
    Network(const std::vector<int>& topology) {
        for (int numNeurons: topology) {
            std::vector<Neuron*> none;
            std::vector<Neuron*> layer;
            for (int i = 0; i < numNeurons; ++i) {
                if (layers.size() == 0) {
                    layer.push_back(new Neuron(none));
                } else {
                    layer.push_back(new Neuron(layers.back()));
                }
            }
            layer.push_back(new Neuron(none));
            layer.back()->setOutput(1.0);

            layers.push_back(layer);
        }
    }

    void setInput(const std::vector<float>& inputs) {
        for (uint i = 0; i < inputs.size(); ++i) {
            layers[0][i]->setOutput(inputs[i]);
        }
    }

    void feedForward() {
        for (uint i=1; i<layers.size(); ++i) {
            for (Neuron* neuron: layers[i]) {
                neuron->feedForward();
            }
        }
    }

    void backPropagate(const std::vector<float>& target, float eta = 0.001, float alpha = 0.01) {
        const int last = layers.size() - 1;
        for (uint i = 0; i < target.size(); ++i) {
            layers[last][i]->setError(target[i] - layers[last][i]->getOutput());
        }

        for (uint i = layers.size() - 1; i == 0; --i) {
            for (Neuron* neuron: layers[i]) {
                neuron->backPropagate(eta, alpha);
            }
        }
    }

    float getError(const std::vector<float>& target) {
        float err = 0.0;
        const int last = layers.size() - 1;
        for (uint i = 0; i < target.size(); ++i) {
            float e = target[i] - layers[last][i]->getOutput();
            err += e*e;
        }
        return sqrt(err/target.size());
    }

    std::vector<float> getResult() {
        std::vector<float> res;
        const int last = layers.size() - 1;
        for (Neuron* neuron: layers[last]) {
            res.push_back(neuron->getOutput());
        }
        res.pop_back();
        return res;
    }

    std::vector<float> getThResult() {
        std::vector<float> res;
        const int last = layers.size() - 1;
        for (Neuron* neuron: layers[last]) {
            float output = neuron->getOutput();
            if (output > 0.5)
                output = 1.0;
            else
                output = 0.0;
            res.push_back(output);
        }
        res.pop_back();
        return res;
    }

private:
    std::vector< std::vector<Neuron*> > layers;
};

} // namespace bacchus

