# IDX to ARFF converter (MNIST dataset converter for Weka)

A simple tool written in C for converting MNIST dataset file format to Weka .arff

![MNIST image](README.md_images/MnistExamples.png)

|License|Issues|
|---------|----------|
| [![License MIT Button](https://img.shields.io/badge/License-MIT-green.svg)](https://github.com/subwave07/MNIST-Weka/blob/master/LICENSE) | [![Report Problems Button](https://img.shields.io/badge/Report-Problems-red.svg)](https://github.com/subwave07/MNIST-Weka/issues)|

## About this project
During year 2018 I had to work on MNIST dataset using Weka. Unfortunately, the dataset was provided in a non-standard format (IDX) and required a converter in order to open it with Weka. I've looked around the web in order to find a premade converter, but the ones available on the [Weka Sourceforge page](https://sourceforge.net/projects/weka/files/datasets/MNIST/) didn't work and were updated in year 2005 (13 years ago).
I found [a converter written in Java](https://github.com/aywi/hdr-mnist-weka/blob/master/src/hdr/mnist/weka/MNISTPreprocessing.java), but it was really slow (10 minutes of processing on a Intel Core i7 3770 - 16GB Ram), so I decided to write my own version in C. The performance improvement is massive (from 10 minutes to 10 seconds).

## Usage
`mnist_784 <dataset name> <dataset labels> <dataset images> <output-file-name>`

e.g. `mnist_784 train train-labels-idx1-ubyte train-images-idx3-ubyte train.arff`

## Resources

* MNIST IDX Dataset File Format: http://yann.lecun.com/exdb/mnist/ (at the bottom)
* Weka ARFF File Format Specifications: https://www.cs.waikato.ac.nz/ml/weka/arff.html

## Acknowledgements

**PhD Yuanyuan Wei** ([aywi](https://github.com/aywi) @ github.com) for his work on a Java converter, which was used as a base for this converter

**Josef Steppan** ([j05s](https://github.com/j05t) @ github.com) for the image at the top of this README
