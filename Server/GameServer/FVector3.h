#pragma once

#include <cmath>
#include <iostream>

struct FVector3
{
    float X;
    float Y;
    float Z;

    // Default constructor
    FVector3() : X(0), Y(0), Z(0) {}

    // Parameterized constructor
    FVector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

    // Copy constructor
    FVector3(const FVector3& other) : X(other.X), Y(other.Y), Z(other.Z) {}

    // Assignment operator
    FVector3& operator=(const FVector3& other)
    {
        if (this != &other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }
        return *this;
    }

    // Vector addition
    FVector3 operator+(const FVector3& other) const
    {
        return FVector3(X + other.X, Y + other.Y, Z + other.Z);
    }

    // Vector subtraction
    FVector3 operator-(const FVector3& other) const
    {
        return FVector3(X - other.X, Y - other.Y, Z - other.Z);
    }

    // Vector scalar multiplication
    FVector3 operator*(float scalar) const
    {
        return FVector3(X * scalar, Y * scalar, Z * scalar);
    }

    // Vector scalar division
    FVector3 operator/(float scalar) const
    {
        if (scalar != 0)
        {
            return FVector3(X / scalar, Y / scalar, Z / scalar);
        }
        else
        {
            // Handle division by zero appropriately
            throw std::runtime_error("Division by zero");
        }
    }

    // Dot product
    float Dot(const FVector3& other) const
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    // Cross product
    FVector3 Cross(const FVector3& other) const
    {
        return FVector3(
            Y * other.Z - Z * other.Y,
            Z * other.X - X * other.Z,
            X * other.Y - Y * other.X);
    }

    // Magnitude (length) of the vector
    float Magnitude() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    // Normalization
    FVector3 Normalize() const
    {
        float mag = Magnitude();
        if (mag > 0)
        {
            return FVector3(X / mag, Y / mag, Z / mag);
        }
        else
        {
            // Handle zero magnitude vector appropriately
            throw std::runtime_error("Cannot normalize zero magnitude vector");
        }
    }

    // Print vector
    void Print() const
    {
        std::cout << "FVector3(" << X << ", " << Y << ", " << Z << ")" << std::endl;
    }
};