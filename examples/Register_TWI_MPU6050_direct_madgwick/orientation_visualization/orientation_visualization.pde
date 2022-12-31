import processing.serial.*;

Serial myPort;
float pitch = 0;
float roll = 0;
float smooth = 0.9;

void setup() {
  myPort = new Serial(this, "COM20", 115200);
  size(640, 480, P3D);
  background(0);
  lights();
  noStroke();
}

void draw() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readStringUntil(0x0A);   
    if (inBuffer != null) {
      String[] parts = trim(inBuffer).split("=");
      if (parts.length == 2) {
        String[] values = trim(parts[1]).split(",");
        if (values.length == 4) {
          float qw = float(trim(values[1]));
          float qx = float(trim(values[3]));
          float qy = float(trim(values[2]));
          float qz = float(trim(values[0]));
          Quaternion q = new Quaternion(qx, qy, qz, qw);

          background(0);
          textSize(24);
          fill(255);
          text("QW=" + qw, 8, 472);
          text("QX=" + qx, 158, 472);
          text("QY=" + qy, 308, 472);
          text("QZ=" + qz, 458, 472);
          //text("Pitch=" + pitch*180/PI, 8, 36);
          //text("Roll=" + pitch*180/PI, 8, 72);

          pushMatrix();
          translate(width/2, height/2, 0);
          applyMatrix(q.toMatrix());
          scale(100);
          beginShape(QUADS); fill(255,  0,  0); vertex(-1, -1,  1); vertex( 1, -1,  1); vertex( 1,  1,  1); vertex(-1,  1,  1); endShape(); // front
          beginShape(QUADS); fill(255,255,  0); vertex( 1, -1, -1); vertex(-1, -1, -1); vertex(-1,  1, -1); vertex( 1,  1, -1); endShape(); // back
          beginShape(QUADS); fill(255,  0,255); vertex(-1,  1,  1); vertex( 1,  1,  1); vertex( 1,  1, -1); vertex(-1,  1, -1); endShape(); // bottom
          beginShape(QUADS); fill(  0,255,  0); vertex(-1, -1, -1); vertex( 1, -1, -1); vertex( 1, -1,  1); vertex(-1, -1,  1); endShape(); // top
          beginShape(QUADS); fill(  0,  0,255); vertex( 1, -1,  1); vertex( 1, -1, -1); vertex( 1,  1, -1); vertex( 1,  1,  1); endShape(); // right
          beginShape(QUADS); fill(  0,255,255); vertex(-1, -1, -1); vertex(-1, -1,  1); vertex(-1,  1,  1); vertex(-1,  1, -1); endShape(); // left
          popMatrix();
        }
      }
    }
  }
}
