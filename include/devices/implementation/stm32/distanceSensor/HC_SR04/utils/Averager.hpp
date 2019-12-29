#pragma once 

namespace Devices::DistanceSensor::Implementation::Stm32::HC_SR04::Utils {
    class Averager {
        private:
            const unsigned short &numberOfAveragedItems;
            float* meansurements = nullptr;
            unsigned short actualIndex = 0;

        public:
            Averager(const unsigned short &_numberOfAveragedItems):
                numberOfAveragedItems(_numberOfAveragedItems),
                meansurements(new float[numberOfAveragedItems] { 0 }) {}

            ~Averager() {
                delete[] meansurements;
            }

            void addMeansurement(const float &value) {
                if (actualIndex >= numberOfAveragedItems) {
                    actualIndex = 0;
                }

                meansurements[actualIndex++] = value;
            }

            float getAverage() {
                float total = 0;

                for (unsigned short i = 0; i < numberOfAveragedItems; ++i) {
                    total += meansurements[i];
                }

                return total / numberOfAveragedItems;
            }
    };
}