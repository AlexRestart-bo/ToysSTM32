#!/bin/bash

set -euo pipefail

echo -e "Очистка решения\t$(date)"
make clean

echo "Создание директории 'build' "
mkdir build

echo "Сборка решения"
make

echo "Прошивка железа .hex-файлом"
make flash

echo "Конец"