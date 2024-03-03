// << DEBUG >>

void debug(){

//    Serial.print("dT = "); Serial.print(timestep); Serial.print("\t");
//    Serial.print("joyPassed = "); Serial.print(joyPassed); Serial.print("\t");
//    Serial.print("joyInit = "); Serial.print(joyInit); Serial.print("\t");

    Serial.print("joyX = "); Serial.print(joyPosX); Serial.print("\t");
    Serial.print("joyY = "); Serial.print(joyPosY); Serial.print("\t");
//    Serial.print("joyTx = "); Serial.print(joyInputs_.T.x.filt); Serial.print("\t");
//    Serial.print("joyTy = "); Serial.print(joyInputs_.T.y.filt); Serial.print("\t");
//    Serial.print("joyOx = "); Serial.print(joyInputs_.O.x.filt); Serial.print("\t");
//    Serial.print("joyOy = "); Serial.print(joyInputs_.O.y.filt); Serial.print("\t");
//    Serial.print("joyTxpos = "); Serial.print(joyInputs_.T.x.pos); Serial.print("\t");
//    Serial.print("joyTxhealth = "); Serial.print(joyInputs_.T.x.health/10); Serial.print("\t");
//    Serial.print("joyTyhealth = "); Serial.print(joyInputs_.T.y.health/10); Serial.print("\t"); 
//    Serial.print("joyOxhealth = "); Serial.print(joyInputs_.O.x.health/10); Serial.print("\t");
//    Serial.print("joyOyhealth = "); Serial.print(joyInputs_.O.y.health/10); Serial.print("\t");

//    Serial.print("a= "); Serial.print(joyAngle); Serial.print("  ");
//    Serial.print("r= "); Serial.print(joyRadius); Serial.print("\t");
//    Serial.print("s= "); Serial.print(scale); Serial.print("  ");

    Serial.print("Lm= "); Serial.print(mix_L); Serial.print("  ");
    Serial.print("Rm= "); Serial.print(mix_R); Serial.print("  ");

    Serial.print("LV = "); Serial.print(motorReqVel_L); Serial.print("\t");
    Serial.print("RV = "); Serial.print(motorReqVel_R); Serial.print("\t");

    Serial.print("LV_Filt = "); Serial.print(motorReqVel_filt_L); Serial.print("\t");
    Serial.print("RV_Filt = "); Serial.print(motorReqVel_filt_R); Serial.print("\t");

//    Serial.print("Switch_Onboard = "); Serial.print(JoySwitch_Onboard); Serial.print("\t");
//    Serial.print("Switch_Main = "); Serial.print(JoySwitch_Main); Serial.print("\t");

//    Serial.print("L mA: "); Serial.print(md.getM1CurrentMilliamps()); Serial.print("\t");
//    Serial.print("R mA: "); Serial.println(md.getM2CurrentMilliamps());

//    Serial.print("VCC = "); Serial.print( readVcc(), DEC ); Serial.print("\t");
//    Serial.print("switchLow = "); Serial.print(switchLow); Serial.print("\t");
//    Serial.print("switchHigh = "); Serial.print(switchHigh); Serial.print("\t");

//    Serial.print("PowerLevelPotInput = "); Serial.print(analogRead(PowerLevelPotInput)); Serial.print("\t");
//    Serial.print("potScale = "); Serial.print(potScale); Serial.print("\t");
    
//    Serial.print("RFor = "); Serial.print(motorForward_R); Serial.print("\t");
//    Serial.print("RxCnt = "); Serial.print(zeroCrossingCountR); Serial.print("\t");

//    Serial.print("LFor = "); Serial.print(motorForward_L); Serial.print("\t");
//    Serial.print("LxCnt = "); Serial.print(zeroCrossingCountL); Serial.print("\t");

    Serial.println("");

}
