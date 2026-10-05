//
//  Alert.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI
import CoreLocation

enum Loc {
    case myLocation
    case custom
}

enum QOS: String, CaseIterable {
    case poor
    case fair
    case good
    case great
}

enum daysOfProp: String, CaseIterable, Identifiable {
    case one = "24h"
    case two = "2d"
    case three = "3d"
    case four = "4d"
    case five = "5d"
    case six = "6d"
    case seven = "1w"
    
    var id: Self { self }
}

func days2sec(days: daysOfProp) -> Double{
    switch days {
    case .one: return 86400
    case .two: return 172800
    case .three: return 259200
    case .four: return 345600
    case .five: return 432000
    case .six: return 518400
    case .seven: return 604800
    }
}

struct Alert: View {
    
    @State private var alertSat: String = ""
    @State private var lat: String = ""
    @State private var lon: String = ""
    @State private var alt: String = ""
    @State private var location: Loc = .custom //need to request authorization before managing the current
    @FocusState private var alertFocus: Bool
    @State private var offsetCoord: CGFloat = CGFloat()
    @State private var startAlert = Date()
    @State private var endAlert = Date()
    @State private var daysAlert: daysOfProp = .one
    @State private var onlyVisible: Bool = false
    @State private var expectedQuality: QOS = .fair
    @State private var elevations: [Int] = [0, 5, 10, 15, 20, 25, 30]
    @State private var expectedElevation = 20
    @State private var alertTimes: [Int] = [5, 15, 30, 60]
    @State private var desiredAlertTime = 15
    @State private var alertSaved = false
    @State private var scaleEffectSave = 1.4
    @State private var customLocation = false
    
    var body: some View {
        
        ZStack {
            
            Color.yaleBlue.ignoresSafeArea()
            
            ScrollView {
                
                VStack (alignment: .leading, spacing: -8){
                    Text("Alerts")
                        .foregroundStyle(Color.ivoryMist)
                        .font(.mainTitleFont)
                        .frame(maxWidth: .infinity, alignment: .leading)
                    
                    Text("Set up a new pass alert")
                        .foregroundStyle(Color.ivoryMist)
                        .font(.subtitleAlerts)
                        .kerning(1.5)
                        .frame(maxWidth: .infinity, alignment: .leading)
                }
                .padding(.horizontal, 30)
                .padding(.bottom, 10)
                
                TextField(
                    "",
                    text: $alertSat,
                    prompt: Text("\(Image(systemName: "magnifyingglass")) Satellite name or NORAD").kerning(2).foregroundStyle(Color.ivoryMist.opacity(0.3))
                )
                .focused($alertFocus)
                .foregroundStyle(.white)
                .padding(.horizontal, 10)
                .frame(maxWidth: .infinity)
                .frame(height: 25, alignment: .leading)
                .font(.inputPickerFont)
                .glassEffect(.clear)
                .padding(.top, 15)
                .padding(.horizontal, 30)
                
                HStack {
                    Text("\(Image(systemName: "mappin.and.ellipse")) Custom observer location")
                        .foregroundStyle(Color.ivoryMist)
                        .font(.subtitleAlerts)
                        .kerning(2)
                        .frame(maxWidth: .infinity, alignment: .leading)
                    
                    
                    RoundedRectangle(cornerRadius: 20)
                        .fill(customLocation ? Color.jungleTeal : Color.ivoryMist.opacity(0.3))
                        .frame(width: 40, height: 20)
                        .overlay {
                            RoundedRectangle(cornerRadius: 20)
                                .fill(Color.ivoryMist)
                                .frame(width: 25, height: 20)
                                .offset(x: customLocation ? 7.5 : -10)
                        }
                        .onTapGesture {
                            withAnimation(.easeInOut(duration: 0.5)) {
                                customLocation.toggle()
                            }
                        }
                        .padding(.top, 2)
                }
                .padding(.top, 15)
                .padding(.horizontal, 30)
                
                
                HStack {
                    //                        HStack {
                    //                            Text("CURRENT")
                    //                                .onTapGesture {
                    //                                    withAnimation(.easeInOut(duration: 0.7)) {
                    //                                        location = .myLocation
                    //                                        offsetCoord = CGFloat(-10.0) // vertical coordinate
                    //                                    }
                    //                                }
                    //                                .animation(.easeInOut(duration: 0.3), value: location)
                    //                                .foregroundStyle(
                    //                                    (location == .myLocation) ? Color.black.opacity(0.8) : Color.black.opacity(0.3)
                    //                                )
                    //                                .font(Font.locationToggle)
                    //
                    //                            Text("•")
                    //                                .foregroundStyle(Color.black).opacity(0.4)
                    //                                .font(Font.locationToggle)
                    //
                    //
                    //                            Text("CUSTOM")
                    //                                .onTapGesture {
                    //                                    withAnimation(.easeInOut(duration: 0.7)) {
                    //                                        location = .custom
                    //                                        offsetCoord = CGFloat(0.0)
                    //                                    }
                    //                                }
                    //                                .animation(.easeInOut(duration: 0.3), value: location)
                    //                                .foregroundStyle(
                    //                                    (location == .custom) ? Color.black.opacity(0.8) : Color.black.opacity(0.3)
                    //                                )
                    //                                .font(Font.locationToggle)
                    //
                    //                        }
                    //                        .padding(.bottom, 3)
                    //                        .padding(.top, 5)
                    //                        .frame(maxWidth: .infinity)
                    //                        .glassEffect(.regular.tint(.white.opacity(0.7)))
                    //                        .padding(.top, 3)
                    
                    
                    
                    if (customLocation) {
                        HStack (spacing: 10) {
                            
                            TextField(
                                "",
                                text: $lat,
                                prompt: Text("Lat".uppercased()).kerning(2).foregroundStyle(Color.ivoryMist.opacity(0.4))
                            )
                            .focused($alertFocus)
                            .keyboardType(.numberPad)
                            .font(.locationToggle)
                            .multilineTextAlignment(.center)
                            .frame(width: 60, height: 25)
                            .glassEffect(.clear, in: .rect(cornerRadius: 20))
                            
                            TextField(
                                "",
                                text: $lon,
                                prompt: Text("Lon".uppercased()).kerning(2).foregroundStyle(Color.ivoryMist.opacity(0.4))
                            )
                            .focused($alertFocus)
                            .keyboardType(.numberPad)
                            .font(.locationToggle)
                            .multilineTextAlignment(.center)
                            .frame(width: 60, height: 25)
                            .glassEffect(.clear, in: .rect(cornerRadius: 20))
                            
                            TextField(
                                "",
                                text: $alt,
                                prompt: Text("Alt".uppercased()).kerning(2).foregroundStyle(Color.ivoryMist.opacity(0.4))
                            )
                            .focused($alertFocus)
                            .keyboardType(.numberPad)
                            .font(.locationToggle)
                            .multilineTextAlignment(.center)
                            .frame(width: 60, height: 25)
                            .glassEffect(.clear, in: .rect(cornerRadius: 20))
                        }
                        .padding(.leading, 55)
                        .frame(maxWidth: .infinity, alignment: .leading)
                        .transition(.offset(y: -10).combined(with: .opacity))
                        .padding(.top, -5)
                    }
                    
                }
                
                Text("\(Image(systemName: "ellipsis.calendar")) Active window")
                    .foregroundStyle(Color.ivoryMist)
                    .font(.subtitleAlerts)
                    .kerning(2)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.top, 25)
                    .padding(.horizontal, 30)
                
//                HStack {
//                    Text("Start")
//                        .font(Font.locationToggle)
//                        .foregroundStyle(Color.ivoryMist)
//                        .kerning(1)
//                    
//                    Spacer()
//                    
//                    DatePicker(
//                        "",
//                        selection: $startAlert,
//                        displayedComponents: [.date]
//                    )
//                    .tint(.darkSlateGrey)
//                    .colorScheme(.dark)
//                    .colorMultiply(.ivoryMist)
//                }
//                .padding(.vertical, 5)
//                .padding(.leading, 60)
//                .padding(.trailing, 30)
//                .padding(.top, -20)
                
                HStack {
                    // Days of propagation for alerts
                    Picker("", selection: $daysAlert) {
                        ForEach(daysOfProp.allCases) { d in
                            Text("\(d.rawValue)")
                                .foregroundStyle(Color.darkSlateGrey)
                        }
                    }
                    .glassEffect(.clear.tint(Color.white.opacity(0.5)))
                    .pickerStyle(.segmented)
                    
                }
                .frame(maxWidth: .infinity)
                .padding(.horizontal, 30)
                .padding(.top, -5)
                .padding(.bottom, 20)
                
                VStack (alignment: .leading, spacing: 0) {
                    
                    
                    HStack (alignment: .center) {
                        Text("Only naked-eye visible passes")
                            .font(Font.locationToggle)
                            .foregroundStyle(Color.ivoryMist)
                            .kerning(1.8)
                        
                        Spacer()
                        
                        RoundedRectangle(cornerRadius: 20)
                            .fill(onlyVisible ? Color.forestGreen : Color.ivoryMist.opacity(0.3))
                            .frame(width: 40, height: 20)
                            .overlay {
                                RoundedRectangle(cornerRadius: 20)
                                    .fill(Color.ivoryMist)
                                    .frame(width: 25, height: 20)
                                    .offset(x: onlyVisible ? 7.5 : -10)
                            }
                            .onTapGesture {
                                withAnimation(.easeInOut(duration: 0.5)) {
                                    onlyVisible.toggle()
                                }
                            }
                    }
                    .padding(.vertical, 5)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    
                    Rectangle()
                        .fill(Color.ivoryMist.opacity(0.3))
                        .frame(width: 350, height: 0.5)
                        .padding(.vertical, 5)
                    
                    HStack {
                        Text("Min. quality")
                            .font(Font.locationToggle)
                            .foregroundStyle(Color.ivoryMist)
                            .kerning(1)
                        
                        Spacer()
                        
                        Picker("", selection: $expectedQuality) {
                            ForEach(QOS.allCases, id: \.self) { qos in
                                Text("\(qos)".capitalized)
                            }
                        }
                        .tint(Color.black)
                        .glassEffect(.clear.tint(Color.white.opacity(0.5)))
                        .pickerStyle(.segmented)
                        .frame(width: 200)
                    }
                    .padding(.vertical, 5)
                    .frame(maxWidth: .infinity)
                    
                    Rectangle()
                        .fill(Color.ivoryMist.opacity(0.3))
                        .frame(width: 350, height: 0.5)
                        .padding(.vertical, 5)
                    
                    HStack {
                        Text("Elevation mask")
                            .font(Font.locationToggle)
                            .foregroundStyle(Color.ivoryMist)
                            .kerning(1)
                        
                        Spacer()
                        
                        Picker("", selection: $expectedElevation) {
                            ForEach(0..<elevations.count, id: \.self) { index in
                                Text("\(elevations[index].description)")
                                    .tag(elevations[index])
                            }
                        }
                        .tint(Color.black)
                        .glassEffect(.clear.tint(Color.white.opacity(0.5)))
                        .pickerStyle(.segmented)
                        .frame(width: 200)
                    }
                    .padding(.vertical, 5)
                    
                    Rectangle()
                        .fill(Color.ivoryMist.opacity(0.3))
                        .frame(width: 350, height: 0.5)
                        .padding(.vertical, 5)
                    
                    HStack {
                        Text("Alert lead time")
                            .font(Font.locationToggle)
                            .foregroundStyle(Color.ivoryMist)
                            .kerning(1)
                        
                        Spacer()
                        
                        Picker("", selection: $desiredAlertTime) {
                            ForEach(0..<alertTimes.count, id: \.self) { index in
                                if (alertTimes[index].description == "60") {
                                    Text("1h")
                                        .tag(alertTimes[index])
                                } else {
                                    Text("\(alertTimes[index].description)m")
                                        .tag(alertTimes[index])
                                }
                            }
                        }
                        .tint(Color.black)
                        .glassEffect(.clear.tint(Color.white.opacity(0.5)))
                        .pickerStyle(.segmented)
                        .frame(width: 200)
                    }
                    .padding(.vertical, 5)
                    
                }
                .padding(.horizontal, 30)
                
                HStack {
                    Text("Generate Alerts")
                        .font(Font.generateAlert)
                        .kerning(5)
                        .foregroundStyle(Color.ivoryMist)

                    if alertSaved {
                        Image(systemName: "checkmark.circle")
                            .scaleEffect(1.3)
                            .foregroundStyle(Color.ivoryMist)
                            .transition(.offset(x: -10).combined(with: .opacity))
                    }
                }
                .padding(.horizontal, 12)
                .padding(.vertical, 3)
                .frame(width: 270)
                .glassEffect(.clear)
                .padding(.top, 15)
                .onTapGesture {
                    withAnimation(.spring(duration: 0.8)) {
                        alertSaved = true
                    }
                }
                
                if (alertSaved) {
                    RoundedRectangle(cornerRadius: 15)
                        .fill(Color.pastelGreen)
                        .frame(maxWidth: .infinity)
                        .frame(height: 80)
                        .overlay {
                            VStack (alignment: .leading) {
                                HStack {
                                    Image(systemName: "checkmark.circle")
                                    Text("Alert Saved").fontWeight(.bold)
                                }
                                .foregroundStyle(Color.forestGreen)
    
                                Text("You'll receive your next alert on DDD, X MMM at HH:MM")
                                    .font(Font.custom("", size: 15))
                                    .multilineTextAlignment(.leading)
                            }
                            .padding(.horizontal, 5)
                        }
                        .transition(.offset(y: -10).combined(with: .opacity))
                        .padding(.horizontal, 30)
                        .padding(.top, 6)
                }
                
                
            }
            .padding(.bottom, 15)
        }
        .onTapGesture {
            alertFocus = false
        }
        
    }
}

#Preview {
    Alert()
}
