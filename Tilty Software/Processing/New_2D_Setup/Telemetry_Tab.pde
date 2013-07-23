Textlabel yaw_label, pitch_label, roll_label, batt_label, alt_label, temp_label;
DropdownList serial_conn;

void setupTelemetryTab() {
  color background = color(100, 100, 100);
  color Label = color(255);
  color active = color(127, 127, 127);
  
  
  gui.getTab("default")
     .setLabel("Main")
     .setColorBackground(background)
     .setColorLabel(Label)
     .setColorActive(active)
     .getCaptionLabel().setFont(tabFont)
     ;
     
  serial_conn = gui.addDropdownList("serial")
                   .setPosition(int(2*scale), int(13*scale))
                   .setSize(int(75*scale), int(100*scale))
                   .setBarHeight(int(6*scale))
                   .setScrollbarVisible(true)
                   .addItems(Serial.list())
                   .setLabel("Select Serial Port")
                   .setColorActive(0)
                   .setColorForeground(color(200,200,0))
                   ;
  serial_conn.captionLabel().style().marginTop = serial_conn.getBarHeight() / 2 - 6;
  serial_conn.addItem("Attempt to auto-connect", -1);
                
  
  yaw_label = gui.addTextlabel("yawLabel")
                 .setText("Yaw: " + str(yaw))
                 .setPosition(width/2 - int(16*scale), int(150*scale))
                 .setColor(255)
                 .setFont(tabFont);
                 ;
                 
  pitch_label = gui.addTextlabel("pitchLabel")
                 .setText("Pitch: " + str(pitch))
                 .setPosition(width/2 - int(16*scale), int(155*scale))
                 .setColor(255)
                 .setFont(tabFont);
                 ;
                 
  roll_label = gui.addTextlabel("rollLabel")
                 .setText("Roll: " + str(roll))
                 .setPosition(width/2 - int(16*scale), int(160*scale))
                 .setColor(255)
                 .setFont(tabFont);
                 ;
  
  alt_label = gui.addTextlabel("altLabel")
                 .setText("Altitude: " + str(altitude))
                 .setPosition(width/2 - int(16*scale), int(165*scale))
                 .setColor(255)
                 .setFont(tabFont);
                 ;
                 
  temp_label = gui.addTextlabel("tempLabel")
                 .setText("Temperature: " + str(temperature))
                 .setPosition(width/2 - int(16*scale), int(170*scale))
                 .setColor(255)
                 .setFont(tabFont);
                 ;
  
  batt_label = gui.addTextlabel("battLabel")
                  .setText("Voltage: " + str(batt_voltage))
                  .setPosition(width - int(30*scale), int(2*scale))
                  .setColor(255)
                  .setFont(tabFont);
                  ;
                  
  gui.addButton("zero", 1)
     .setPosition(width/2 - 43, int(176*scale))
     ;
}


void drawTelemetry() {
  if (gui.getTab("default").isActive()) {
    yaw_label.setText("Yaw: " + str(yaw) + "°");
    pitch_label.setText("Pitch: " + str(pitch) + "°");
    roll_label.setText("Roll: " + str(roll) + "°");
    batt_label.setText("Voltage:\n" + str(batt_voltage) + "v");
    alt_label.setText("Altitude: " + str(altitude) + " m");
    temp_label.setText("Temperature: " + str(temperature) + "°C");
    
    //  Battery voltage Meter 
    pushMatrix();
    //batt_voltage = abs(sin(frameCount * 0.025) * 24);
    stroke(255);
    fill(10);
    rect(width - 5*scale, 25*scale, 10, -24 * scale);
    noStroke();
    if (batt_voltage > min_batt) {  fill(0, 255, 0);}
    else {  fill(255, 0, 0);}
    rect(width - 5*scale, 25*scale, 10, constrain(-map(batt_voltage, 0, max_batt, 0, 24*scale), -24*scale, 0));
    popMatrix();
  }
}

void zero(int value) {
  if (myPort != null) {
    myPort.write('Z');
    myPort.write('Y');
    myPort.write('P');
    myPort.write('Z');
    myPort.write('A');
  }
}