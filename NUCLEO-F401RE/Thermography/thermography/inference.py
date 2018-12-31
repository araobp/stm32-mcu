import numpy as np
from keras import models
import yaml
#import sklearn.preprocessing as pp

# Normalization
def scale(data):
    min_ = np.min(data)
    max_ = np.max(data)
    mean = (max_ + min_) / 2.0
    range_ = (max_ - min_) / 2.0
    data = (data - mean)/range_
    return data

# Trained CNN model
class Model:

    def __init__(self, shape, class_file, model_file):

        self.shape = shape
        self.class_labels = None
        self.activation_model = None

        with open(class_file, 'r') as f:
            self.class_labels = yaml.load(f)
            _model = models.load_model(model_file)
            _model.summary()
            layer_outputs = [layer.output for layer in _model.layers]
            self.activation_model = models.Model(inputs=_model.input, outputs=layer_outputs)
    
    def infer(self, data):
        if self.activation_model:
            activations = self.activation_model.predict([scale(data).reshape(1,self.shape[0],self.shape[1],1)])
            result = (activations[-1]*100)  # The last layer
            p = result[0]
            max_idx = np.argmax(p*100)
            return (self.class_labels[max_idx], p[max_idx])
        else:
            return None
