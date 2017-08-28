//
// Created by ascadian on 8/9/17.
//

bool operator==(const cam_intrinsics &lhs, const cam_intrinsics &rhs) {
    return lhs.ppx == rhs.ppx
           && lhs.ppy == rhs.ppy
           && lhs.fx == rhs.fx
           && lhs.fy == rhs.fy
           && lhs.coeffs[0] == rhs.coeffs[0]
           && lhs.coeffs[1] == rhs.coeffs[1]
           && lhs.coeffs[2] == rhs.coeffs[2]
           && lhs.coeffs[3] == rhs.coeffs[3]
           && lhs.coeffs[4] == rhs.coeffs[4];
}