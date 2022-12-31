// https://behreajj.medium.com/3d-rotations-in-processing-vectors-matrices-quaternions-10e2fed5f0a3

class Quaternion {
  float x = 0.0; float y = 0.0; float z = 0.0;
  float w = 1.0;

  Quaternion() { }

  Quaternion(float x, float y, float z, float w) {
    this.x = x; this.y = y; this.z = z; this.w = w;
  }

  Quaternion(Quaternion q) {
    x = q.x; y = q.y; z = q.z; w = q.w;
  }

  // Construct from axis-angle.
  Quaternion(float angle, PVector axis) {
    set(angle, axis);
  }

  String toString() {
    return String.format("[ %+.2f, %+.2f, %+.2f, %+.2f ]",
      x, y, z, w);
  }

  Quaternion add(Quaternion q) {
    x += q.x; y += q.y; z += q.z; w += q.w;
    return this;
  }

  boolean approx(Quaternion q, float tolerance) {
    return abs(x - q.x) <= tolerance
      && abs(y - q.y) <= tolerance
      && abs(z - q.z) <= tolerance
      && abs(w - q.w) <= tolerance;
  }

  float dot(Quaternion b) {
    return x * b.x + y * b.y + z * b.z + w * b.w;
  }

  Quaternion mult(float scalar) {
    x *= scalar; y *= scalar; z *= scalar; w *= scalar;
    return this;
  }

  // Apply to point.
  PVector mult(PVector v) {
    return mult(v, new PVector());
  }

  PVector mult(PVector v, PVector out) {
    float ix = w * v.x + y * v.z - z * v.y;
    float iy = w * v.y + z * v.x - x * v.z;
    float iz = w * v.z + x * v.y - y * v.x;
    float iw = -x * v.x - y * v.y - z * v.z;
    out.x = ix * w + iw * -x + iy * -z - iz * -y;
    out.y = iy * w + iw * -y + iz * -x - ix * -z;
    out.z = iz * w + iw * -z + ix * -y - iy * -x;
    return out;
  }

  Quaternion normalize() {
    float mag = x * x + y * y + z * z + w * w;
    if (mag != 0.0 && mag != 1.0) {
      mag = 1.0 / sqrt(mag);
      x *= mag; y *= mag; z *= mag; w *= mag;
    }
    return this;
  }

  Quaternion rescale(float scalar) {
    float mag = x * x + y * y + z * z + w * w;
    if (mag == 0.0) {
      return this;
    } else if (mag == 1.0) {
      x *= scalar; y *= scalar; z *= scalar; w *= scalar;
      return this;
    }
    mag = scalar / sqrt(mag);
    x *= mag; y *= mag; z *= mag; w *= mag;
    return this;
  }

  Quaternion set(float x, float y, float z, float w) {
    this.x = x; this.y = y; this.z = z; this.w = w;
    return this;
  }

  Quaternion set(Quaternion q) {
    x = q.x; y = q.y; z = q.z; w = q.w;
    return this;
  }

  // Set from axis-angle.
  Quaternion set(float angle, PVector axis) {
    float halfangle = 0.5 * angle;
    float sinhalf = sin(halfangle);
    x = axis.x * sinhalf; y = axis.y * sinhalf;
    z = axis.z * sinhalf; w = cos(halfangle);
    return this;
  }

  Quaternion sub(Quaternion q) {
    x -= q.x; y -= q.y; z -= q.z; w -= q.w;
    return this;
  }
  
  Quaternion slerp(Quaternion a, Quaternion b, float step) {
    // Return early if step is out of bounds [0, 1].
    if (step <= 0.0) { 
      return set(a);
    } else if (step >= 1.0) { 
      return set(b);
    }

    // dot product = cos(t).
    float cos = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    // Prefer the shortest distance by flipping
    // the destination's sign if cos(t) is negative.
    if (cos < 0.0) {
      x = -b.x; y = -b.y; z = -b.z; w = -b.w; cos = -cos;
    } else {
      x = b.x; y = b.y; z = b.z; w = b.w;
    }

    // If cosine is out of bounds, return the origin.
    if (cos >= 1.0) {
      x = a.x; y = a.y; z = a.z; w = a.w;
      return this;
    }

    float sin = sqrt(1.0 - cos * cos);
    if (abs(sin) < EPSILON) {
      w = 0.5 * (a.w + w);
      x = 0.5 * (a.x + x);
      y = 0.5 * (a.y + y);
      z = 0.5 * (a.z + z);

      return this;
    }

    // Interpolation.
    float theta = atan2(sin, cos);
    float u = sin((1.0 - step) * theta) / sin;
    float t = sin(step * theta) / sin;
    x = u * a.x + t * b.x;
    y = u * a.y + t * b.y;
    z = u * a.z + t * b.z;
    w = u * a.w + t * b.w;
    return this;
  }
  
  PMatrix3D toMatrix() {
    return toMatrix(new PMatrix3D());
  }

  PMatrix3D toMatrix(PMatrix3D out) {
    float x2 = x + x; float y2 = y + y; float z2 = z + z;
    float xsq2 = x * x2; float ysq2 = y * y2; float zsq2 = z * z2;
    float xy2 = x * y2; float xz2 = x * z2; float yz2 = y * z2;
    float wx2 = w * x2; float wy2 = w * y2; float wz2 = w * z2;
    out.set(
      1.0 - (ysq2 + zsq2), xy2 - wz2, xz2 + wy2, 0.0,
      xy2 + wz2, 1.0 - (xsq2 + zsq2), yz2 - wx2, 0.0,
      xz2 - wy2, yz2 + wx2, 1.0 - (xsq2 + ysq2), 0.0,
      0.0, 0.0, 0.0, 1.0);
    return out;
  }
  
  Quaternion(PMatrix3D m) {
    set(m);
  }

  Quaternion(float ix, float jx, float kx,
    float iy, float jy, float ky,
    float iz, float jz, float kz) {
    set(ix, jx, kx, iy, jy, ky, iz, jz, kz);
  }

  Quaternion set(PMatrix3D m) {
    return set(m.m00, m.m01, m.m02,
      m.m10, m.m11, m.m12,
      m.m20, m.m21, m.m22);
  }

  Quaternion set(float ix, float jx, float kx,
    float iy, float jy, float ky,
    float iz, float jz, float kz) {
    w = sqrt(max(0.0, 1.0 + ix + jy + kz)) * 0.5;
    x = sqrt(max(0.0, 1.0 + ix - jy - kz)) * 0.5;
    y = sqrt(max(0.0, 1.0 - ix + jy - kz)) * 0.5;
    z = sqrt(max(0.0, 1.0 - ix - jy + kz)) * 0.5;
    x *= Math.signum(jz - ky);
    y *= Math.signum(kx - iz);
    z *= Math.signum(iy - jx);
    return this;
  }
}
