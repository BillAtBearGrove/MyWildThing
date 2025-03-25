// << DEBUG >>

void debug(){

// Check Keystrokes
if (Serial.available()) {
  switch (Serial.read())
  {
    case 't': echoJoyTether?  echoJoyTether=false  : echoJoyTether=true;  break;
    case 'o': echoJoyOnboard? echoJoyOnboard=false : echoJoyOnboard=true; break;
    case 'l': echoMotorL? echoMotorL=false : echoMotorL=true; break;
    case 'r': echoMotorR? echoMotorR=false : echoMotorR=true; break;
    case 'd': echoDT?  echoDT=false  : echoDT=true;  break;
    case 'p': echoPID? echoPID=false : echoPID=true; break;
    case 'a': echoAR?  echoAR=false  : echoAR=true;  break;
  }
}

  if (echoDT){
    Serial.print("dT = "); Serial.print(timestep); Serial.print("  ");
  }
//    Serial.print("joyPassed = "); Serial.print(joyPassed); Serial.print("  ");
//    Serial.print("joyInit = "); Serial.print(joyInit); Serial.print("  ");

  if (echoJoyTether){
    Serial.print("xraw = "); Serial.print(joyInputs_.T.x.raw); Serial.print("  ");
    Serial.print("yraw = "); Serial.print(joyInputs_.T.y.raw); Serial.print("  ");
    Serial.print("xf = "); Serial.print(joyInputs_.T.x.filt); Serial.print("  ");
    Serial.print("yf = "); Serial.print(joyInputs_.T.y.filt); Serial.print("  "); 
//    Serial.print("xref = "); Serial.print(joyInputs_.T.x.ref); Serial.print("  ");
//    Serial.print("yref = "); Serial.print(joyInputs_.T.y.ref); Serial.print("  "); 
    Serial.print("OK2L = "); Serial.print(joyInputs_.T.isOK2Learn); Serial.print("  ");
    Serial.print("LCount = "); Serial.print(joyInputs_.T.learnCount); Serial.print("  ");
    Serial.print("xc = "); Serial.print(joyInputs_.T.x.center); Serial.print("  ");
    Serial.print("yc = "); Serial.print(joyInputs_.T.y.center); Serial.print("  ");
//    Serial.print("xp = "); Serial.print(joyInputs_.T.x.pos); Serial.print("  ");
//    Serial.print("yp = "); Serial.print(joyInputs_.T.y.pos); Serial.print("  ");
//    Serial.print("xH = "); Serial.print(joyInputs_.T.x.health); Serial.print("  ");
//    Serial.print("yH = "); Serial.print(joyInputs_.T.y.health); Serial.print("  ");
    Serial.print("stat = "); Serial.print(joyInputs_.T.status); Serial.print("  ");
  }
  if (echoJoyOnboard){
    Serial.print("xraw = "); Serial.print(joyInputs_.O.x.raw); Serial.print("  ");
    Serial.print("yraw = "); Serial.print(joyInputs_.O.y.raw); Serial.print("  ");
    Serial.print("xf = "); Serial.print(joyInputs_.O.x.filt); Serial.print("  ");
    Serial.print("yf = "); Serial.print(joyInputs_.O.y.filt); Serial.print("  "); 
//    Serial.print("xref = "); Serial.print(joyInputs_.O.x.ref); Serial.print("  ");
//    Serial.print("yref = "); Serial.print(joyInputs_.O.y.ref); Serial.print("  "); 
    Serial.print("OK2L = "); Serial.print(joyInputs_.O.isOK2Learn); Serial.print("  ");
    Serial.print("LCount = "); Serial.print(joyInputs_.O.learnCount); Serial.print("  ");
    Serial.print("xc = "); Serial.print(joyInputs_.O.x.center); Serial.print("  ");
    Serial.print("yc = "); Serial.print(joyInputs_.O.y.center); Serial.print("  ");
//    Serial.print("xp = "); Serial.print(joyInputs_.O.x.pos); Serial.print("  ");
//    Serial.print("yp = "); Serial.print(joyInputs_.O.y.pos); Serial.print("  ");
//    Serial.print("xH = "); Serial.print(joyInputs_.O.x.health); Serial.print("  ");
//    Serial.print("yH = "); Serial.print(joyInputs_.O.y.health); Serial.print("  ");
    Serial.print("stat = "); Serial.print(joyInputs_.O.status); Serial.print("  ");
  }
  if (echoAR){
    Serial.print("a= "); Serial.print(joyAngle); Serial.print("  ");
    Serial.print("r= "); Serial.print(joyRadius); Serial.print("  ");
  }
//    Serial.print("Mode "); Serial.print(inputMode); Serial.print("  ");
//    Serial.print("s= "); Serial.print(scale); Serial.print("  ");

//    Serial.print("Lm= "); Serial.print(mix_L); Serial.print("  ");
//    Serial.print("Rm= "); Serial.print(mix_R); Serial.print("  ");

//    Serial.print("LV = "); Serial.print(motorReqVel_L); Serial.print("  ");
//    Serial.print("RV = "); Serial.print(motorReqVel_R); Serial.print("  ");

//    Serial.print("motorReqVel_filt_L = "); Serial.print(motorReqVel_filt_L); Serial.print("  ");
//    Serial.print("motorReqVel_filt_R = "); Serial.print(motorReqVel_filt_R); Serial.print("  ");

//    Serial.print("Switch_Onboard = "); Serial.print(JoySwitch_Onboard); Serial.print("  ");
//    Serial.print("Switch_Main = "); Serial.print(JoySwitch_Main); Serial.print("  ");

//    Serial.print("L mA: "); Serial.print(md.getM1CurrentMilliamps()); Serial.print("  ");
//    Serial.print("R mA: "); Serial.println(md.getM2CurrentMilliamps());

//    Serial.print("VCC = "); Serial.print( readVcc(), DEC ); Serial.print("  ");
//    Serial.print("switchLow = "); Serial.print(switchLow); Serial.print("  ");
//    Serial.print("switchHigh = "); Serial.print(switchHigh); Serial.print("  ");

//    Serial.print("powerLevelPotInput = "); Serial.print(analogRead(powerLevelPotInput)); Serial.print("  ");
//    Serial.print("potScale = "); Serial.print(potScale); Serial.print("  ");
    
//    Serial.print("RFor = "); Serial.print(motorForward_R); Serial.print("  ");
//    Serial.print("RxCnt = "); Serial.print(zeroCrossingCountR); Serial.print("  ");

//    Serial.print("LFor = "); Serial.print(motorForward_L); Serial.print("  ");
//    Serial.print("LxCnt = "); Serial.print(zeroCrossingCountL); Serial.print("  ");

//    Serial.println("");

}
