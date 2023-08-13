# Use an official Ubuntu as the base image
FROM ubuntu:latest

# Set environment variables for non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update --fix-missing && apt-get install -y \
    cmake \
    g++ \
    git \
    wget \
    zip \
    unzip \
    curl \
    tar \
    libopencv-dev \
    python3-opencv \
    && rm -rf /var/lib/apt/lists/*

# Clone your project's repository
RUN git clone https://github.com/eraldss1/nodeflux-app.git /app/crow-resize-image

# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /app/vcpkg
WORKDIR /app/vcpkg
RUN ./bootstrap-vcpkg.sh

# Install Crow and OpenCV using vcpkg
RUN ./vcpkg install crow

# Build your Crow application
WORKDIR /app/crow-resize-image
RUN mkdir build
WORKDIR /app/crow-resize-image/build
RUN cmake -DCMAKE_TOOLCHAIN_FILE=/app/vcpkg/scripts/buildsystems/vcpkg.cmake ..
RUN cmake --build .

# Expose the port on which your Crow application will listen
EXPOSE 18080

# Run your Crow application
CMD ["./main"]