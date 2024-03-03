// << DEBUG >>

void debug(){

//    Serial.print("dT = "); Serial.print(timestep); Serial.print("  ");
//    Serial.print("joyPassed = "); Serial.print(joyPassed); Serial.print("  ");
//    Serial.print("joyInit = "); Serial.print(joyInit); Serial.print("  ");

//    Serial.print("joyX = "); Serial.print(joyPosX); Serial.print("  ");
//    Serial.print("joyY = "); Serial.print(joyPosY); Serial.print("  ");
//    Serial.print("xraw = "); Serial.print(joyInputs_.T.x.raw); Serial.print("  ");
//    Serial.print("yraw = "); Serial.print(joyInputs_.T.y.raw); Serial.print("  ");
//    Serial.print("xf = "); Serial.print(joyInputs_.T.x.filt); Serial.print("  ");
//    Serial.print("yf = "); Serial.print(joyInputs_.T.y.filt); Serial.print("  "); 
//    Serial.print("xref = "); Serial.print(joyInputs_.T.x.ref); Serial.print("  ");
//    Serial.print("yref = "); Serial.print(joyInputs_.T.y.ref); Serial.print("  "); 
//    Serial.print("OK2L = "); Serial.print(joyInputs_.T.isOK2Learn); Serial.print("  ");
//    Serial.print("LCount = "); Serial.print(joyInputs_.T.learnCount); Serial.print("  ");
//    Serial.print("xc = "); Serial.print(joyInputs_.T.x.center); Serial.print("  ");
//    Serial.print("yc = "); Serial.print(joyInputs_.T.y.center); Serial.print("  ");
//    Serial.print("xp = "); Serial.print(joyInputs_.T.x.pos); Serial.print("  ");
//    Serial.print("yp = "); Serial.print(joyInputs_.T.y.pos); Serial.print("  ");
//    Serial.print("xH = "); Serial.print(joyInputs_.T.x.health); Serial.print("  ");
//    Serial.print("yH = "); Serial.print(joyInputs_.T.y.health); Serial.print("  ");
//    Serial.print("stat = "); Serial.print(joyInputs_.T.status); Serial.print("  ");

    Serial.print("a= "); Serial.print(joyAngle); Serial.print("  ");
    Serial.print("r= "); Serial.print(joyRadius); Serial.print("  ");
//    Serial.print("Mode "); Serial.print(inputMode); Serial.print("  ");
//    Serial.print("s= "); Serial.print(scale); Serial.print("  ");

//    Serial.print("Lm= "); Serial.print(mix_L); Serial.print("  ");
//    Serial.print("Rm= "); Serial.print(mix_R); Serial.print("  ");

//    Serial.print("LV = "); Serial.print(motorReqVel_L); Serial.print("  ");
//    Serial.print("RV = "); Serial.print(motorReqVel_R); Serial.print("  ");

    Serial.print("motorReqVel_filt_L = "); Serial.print(motorReqVel_filt_L); Serial.print("  ");
    Serial.print("motorReqVel_filt_R = "); Serial.print(motorReqVel_filt_R); Serial.print("  ");

//    Serial.print("Switch_Onboard = "); Serial.print(JoySwitch_Onboard); Serial.print("  ");
//    Serial.print("Switch_Main = "); Serial.print(JoySwitch_Main); Serial.print("  ");

//    Serial.print("L mA: "); Serial.print(md.getM1CurrentMilliamps()); Serial.print("  ");
//    Serial.print("R mA: "); Serial.println(md.getM2CurrentMilliamps());

//    Serial.print("VCC = "); Serial.print( readVcc(), DEC ); Serial.print("  ");
//    Serial.print("switchLow = "); Serial.print(switchLow); Serial.print("  ");
//    Serial.print("switchHigh = "); Serial.print(switchHigh); Serial.print("  ");

//    Serial.print("PowerLevelPotInput = "); Serial.print(analogRead(PowerLevelPotInput)); Serial.print("  ");
//    Serial.print("potScale = "); Serial.print(potScale); Serial.print("  ");
    
//    Serial.print("RFor = "); Serial.print(motorForward_R); Serial.print("  ");
//    Serial.print("RxCnt = "); Serial.print(zeroCrossingCountR); Serial.print("  ");

//    Serial.print("LFor = "); Serial.print(motorForward_L); Serial.print("  ");
//    Serial.print("LxCnt = "); Serial.print(zeroCrossingCountL); Serial.print("  ");

//    Serial.println("");

}
