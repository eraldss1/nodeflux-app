# Use an official Ubuntu as the base image
FROM ubuntu:latest

# Set environment variables for non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    g++ \
    git \
    libopencv-dev \
    wget \
    curl \
    zip \
    unzip \
    tar \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git /app/vcpkg
WORKDIR /app/vcpkg
RUN ./bootstrap-vcpkg.sh

# Install Crow using vcpkg
RUN ./vcpkg install crow

# Clone your project's repository
RUN git clone https://github.com/eraldss1/nodeflux-app.git /app/crow_resize_image

# Build your Crow application
WORKDIR /app/crow_resize_image
RUN mkdir build
WORKDIR /app/crow_resize_image/build
RUN cmake -DCMAKE_TOOLCHAIN_FILE=/app/vcpkg/scripts/buildsystems/vcpkg.cmake ..
RUN cmake --build .

# Expose the port on which your Crow application will listen
EXPOSE 18080

# Run your Crow application
CMD ["./main"]
