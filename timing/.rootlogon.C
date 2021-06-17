{   
   TStyle* wptStyle = new TStyle("wptStyle","Plot style for WPT");

  wptStyle->SetLineWidth(2);
  wptStyle->SetTitleSize(.05, "xy");
  wptStyle->SetTitleOffset(0.7, "y");
  wptStyle->SetTitleOffset(1, "x");
//   wptStyle->SetTicks("+-");
  wptStyle->SetTitleFont(22,"xyz"); // font option  
  wptStyle->SetLabelFont(22,"xyz");
  cout << "Styles are Set!" << endl;
  return ;

  }

