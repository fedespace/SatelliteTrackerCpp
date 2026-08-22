//
//  GS.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

struct GS: View {
    
    @Binding var satellite: String
    @Binding var startTime: Date
    @Binding var endTime: Date
    @Binding var inputType: InputOptions
    @Binding var searchItem: String?
    @Binding var gtViewModel: GroundTrackViewModel
    @State var gsLat: String = ""
    @State var gsLon: String = ""
    @State var gsAlt: String = ""
    @State var gsMask: String = ""
    //@State var passModel = PassViewModel()
    @State var passModel: PassViewModel
    @State var maxElevation: String = ""
    @State var duration: String = ""
    @State var showPasses = false
    // To translate the string utc into date utc
    var utcStringToDate: DateFormatter {
        let utcStringToDate = DateFormatter()
        utcStringToDate.dateFormat = "yyyy-M-d H:m:s"
        utcStringToDate.timeZone = TimeZone(identifier: "UTC")
        utcStringToDate.locale = Locale(identifier: "en_US_POSIX")
        return utcStringToDate
    }
    // To translate the date utc to formatted string with only time (for displaying in frontend)
    var utcDateToLocalString: DateFormatter {
        let utcDateToLocalString = DateFormatter()
        utcDateToLocalString.dateFormat = "HH:mm:ss"
        utcDateToLocalString.timeZone = .current
        return utcDateToLocalString
    }
    var utcDateToLocalString_hh_mm: DateFormatter {
        let utcDateToLocalString_hh_mm = DateFormatter()
        utcDateToLocalString_hh_mm.dateFormat = "HH:mm"
        utcDateToLocalString_hh_mm.timeZone = .current
        return utcDateToLocalString_hh_mm
    }
    // To translate the date utc into date string (complete) local
    var utcDateToCompleteLocalString: DateFormatter {
        let utcDateToCompleteLocalString = DateFormatter()
        utcDateToCompleteLocalString.dateFormat = "yyyy-M-d H:m:s"
        utcDateToCompleteLocalString.timeZone = .current
        return utcDateToCompleteLocalString
    }
    // To translate the complete local string to complete local date
    var df: DateFormatter {
        let df = DateFormatter()
        df.dateFormat = "yyyy-MM-dd HH:mm:ss"
        return df
    }
    // To get time before next AOS pass
    var comp: DateComponentsFormatter {
        let comp = DateComponentsFormatter()
        comp.allowedUnits = [.hour, .minute, .second]
        comp.unitsStyle = .abbreviated
        return comp
    }
    // Future passes
    var futurePass: DateFormatter {
        let futurePass = DateFormatter()
        futurePass.dateFormat = "d MMM"
        return futurePass
    }
    // Time in the inputs
    var timeInput: DateFormatter {
        let timeInput = DateFormatter()
        timeInput.dateFormat = "HH:mm"
        return timeInput
    }
    // Validity of inputs
    @State private var isLatValid = true
    @State private var isLonValid = true
    @State private var isMaskValid = true
    @State private var isAltValid = true
    var isValid: Bool {isLatValid && isLonValid && isMaskValid}
    @State private var showErrorValues = false
    @State private var valueErrorMessage = ""
    
    
    
    
    init(
        satellite: Binding<String>,
        startTime: Binding<Date>,
        endTime: Binding<Date>,
        inputType: Binding<InputOptions>,
        searchItem: Binding<String?>,
        passModel: PassViewModel = PassViewModel(),
        gtViewModel: Binding<GroundTrackViewModel>
    ) {
        self._satellite = satellite
        self._startTime = startTime
        self._endTime = endTime
        self._inputType = inputType
        self._searchItem = searchItem
        self._passModel = State(initialValue: passModel)
        self._gtViewModel = gtViewModel
    }
    
    var body: some View {
        
        
        
        ZStack {
            
            Color.yaleBlue.ignoresSafeArea(.all)
            
            ScrollView {
                
                VStack(spacing: 0) {
                    HStack(spacing: 20) {
                        
                        Image("satellite")
                            .resizable()
                            .foregroundStyle(Color.ivoryMist)
                            .frame(width: 30, height: 30)
                        
                        //Text("gtViewModel.satName")
                        Text("ISS ZARYA")
                            .font(.gsTitle)
                            .kerning(8)
                            .scaleEffect(y: 0.9)
                            .foregroundStyle(Color.ivoryMist)
                    }
                    
                    let fromDateString = futurePass.string(from: $startTime.wrappedValue)
                    let fromTimeString = timeInput.string(from: $startTime.wrappedValue)
                    let toDateString = futurePass.string(from: $endTime.wrappedValue)
                    let toTimeString = timeInput.string(from: $endTime.wrappedValue)
                    
                    Text("\(fromDateString) \(fromTimeString) → \(toDateString) \(toTimeString)")
                        .font(Font.gsWindow)
                        .kerning(1)
                        .foregroundStyle(Color.ivoryMist)
                }
                .frame(maxWidth: .infinity, alignment: .center)
                .padding(.bottom, 20)
                .padding(.top, 5)
                
                GlassEffectContainer {
                    
                    HStack(spacing: 15) {
                        
                        Image("gs")
                            .resizable()
                            .foregroundStyle(Color.darkSlateGrey)
                            .frame(width: 30, height: 30)
                            .padding(.bottom, 4)
                        
                        Text("Ground Station").textCase(.uppercase)
                            .font(.eunomiaRegular)
                            .kerning(8)
                            .foregroundStyle(Color.darkSlateGrey)
                        
                    }
                    .frame(maxWidth: .infinity, alignment: .center)
                    
                    HStack(spacing: 0) {
                        
                        HStack(alignment: .center, spacing: 15) {
                            
                            VStack {
                                
                                VStack(spacing: 2) {
                                    Text("Lat [deg]")
                                        .textCase(.uppercase)
                                        .font(.coord)
                                        .kerning(2)
                                        .foregroundStyle(Color.prussianBlue)
                                    TextField("", text: $gsLat)
                                        .multilineTextAlignment(.center)
                                        .padding(.vertical, 2)
                                        .padding(.horizontal, 15)
                                        .frame(maxWidth: 100)
                                        .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.25)), in: .rect(cornerRadius: 10))
                                        .onChange(of: gsLat) {
                                            guard let lat = Int(gsLat) else { return }
                                            isLatValid = lat >= -90 && lat <= 90
                                            showErrorValues = !isLatValid
                                            valueErrorMessage = "Latitude out of allowed range."
                                        }
                                        .overlay(RoundedRectangle(cornerRadius: 10).stroke(isLatValid ? .clear : .red))
                                }
                                
                                
                                VStack(spacing: 2) {
                                    Text("Alt [m]")
                                        .textCase(.uppercase)
                                        .font(.coord)
                                        .kerning(2)
                                        .foregroundStyle(Color.prussianBlue)
                                    TextField("", text: $gsAlt)
                                        .multilineTextAlignment(.center)
                                        .padding(.vertical, 2)
                                        .padding(.horizontal, 15)
                                        .frame(maxWidth: 100)
                                        .onChange(of: gsAlt) {
                                            guard let alt = Int(gsAlt) else { return }
                                            if (alt < 0) {
                                                isAltValid = false
                                                showErrorValues = !isAltValid
                                                valueErrorMessage = "Altitude must be provided."
                                            } else {
                                                isAltValid = true
                                                showErrorValues = !isAltValid
                                            }
                                        }
                                        .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.25)), in: .rect(cornerRadius: 10))
                                        .overlay(RoundedRectangle(cornerRadius: 10).stroke(isAltValid ? .clear : .red))
                                }
                            }
                            
                            VStack {
                                
                                VStack(spacing: 2) {
                                    Text("Lon [deg]")
                                        .textCase(.uppercase)
                                        .font(.coord)
                                        .kerning(2)
                                        .foregroundStyle(Color.prussianBlue)
                                    TextField("", text: $gsLon)
                                        .multilineTextAlignment(.center)
                                        .padding(.vertical, 2)
                                        .padding(.horizontal, 15)
                                        .frame(maxWidth: 100)
                                        .onChange(of: gsLon) {
                                            guard let lon = Int(gsLon) else { return }
                                            isLonValid = lon >= -180 && lon <= 180
                                            showErrorValues = !isLonValid
                                            valueErrorMessage = "Longitude out of allowed range."
                                        }
                                        .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.25)), in: .rect(cornerRadius: 10))
                                        .overlay(RoundedRectangle(cornerRadius: 10).stroke(isLonValid ? .clear : .red))
                                }
                                
                                
                                VStack(spacing: 2) {
                                    Text("Mask [deg]")
                                        .textCase(.uppercase)
                                        .font(.coord)
                                        .kerning(2)
                                        .foregroundStyle(Color.prussianBlue)
                                    TextField("", text: $gsMask)
                                        .multilineTextAlignment(.center)
                                        .padding(.vertical, 2)
                                        .padding(.horizontal, 15)
                                        .frame(maxWidth:100)
                                        .onChange(of: gsMask) {
                                            if (gsMask == "") {
                                                isMaskValid = false
                                                showErrorValues = !isMaskValid
                                                valueErrorMessage = "Elevation mask must be provided."
                                            } else {
                                                isMaskValid = true
                                                showErrorValues = !isMaskValid
                                            }
                                        }
                                        .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.25)), in: .rect(cornerRadius: 10))
                                        .overlay(RoundedRectangle(cornerRadius: 10).stroke(isMaskValid ? .clear : .red))
                                }
                                
                            }
                            
                        }
                        .frame(maxWidth: .infinity)
                        .padding(.horizontal, 18)
                        
                        // Button to fetch passes
                        var fetchColor = Color.darkSlateGrey.opacity(0.75)
                        RoundedRectangle(cornerRadius: 10)
                            .fill(fetchColor)
                            .overlay(
                                Image(systemName: "arrow.forward")
                                    .foregroundStyle(Color.ivoryMist)
                                    .scaleEffect(x: 0.8, y: 0.9)
                                    .padding(.vertical, 10)
                                    .padding(.horizontal, 10)
                                    .onTapGesture {
                                        if (isValid && gsLat != "" && gsLon != "") {
                                            //passes = []
                                            fetchColor = Color.darkSlateGrey.opacity(0.75)
                                            valueErrorMessage = ""
                                            showPasses = true
                                            Task {
                                                await passModel.fetchPasses(name: gtViewModel.satName, line1: gtViewModel.tleLine1, line2: gtViewModel.tleLine2, startTime: startTime, endTime: endTime, gsLat: gsLat, gsLon: gsLon, gsAlt: gsAlt, gsMask: gsMask, inputType: inputType, searchItem: searchItem!)
                                            }
                                            if (passModel.passes.first == nil) {
                                                Text("No passes in the selected time window.")
                                            }
                                        } else {
                                            showPasses = false
                                            showErrorValues = true
                                            isLatValid = (gsLat == "") ? false : isLatValid
                                            isLonValid = (gsLon == "") ? false : isLonValid
                                            isAltValid = (gsAlt == "") ? false : isAltValid
                                            isMaskValid = (gsMask == "") ? false : isMaskValid
                                            valueErrorMessage = (gsLat == "" || gsLon == "") ? "Missing input" : valueErrorMessage
                                            fetchColor = Color.gray.opacity(0.4)
                                        }
                                        
                                    }
                            )
                            .frame(width: 35, height: 90)
                            .padding(.top, 15)
                    }
                    .frame(maxWidth: .infinity)
                    .padding(.trailing, 15)
                    
                    
                }
                .padding(.horizontal, 10)
                .padding(.top, 10)
                .padding(.bottom, 15)
                .frame(maxWidth: .infinity, alignment: .leading)
                .glassEffect(.regular.tint(Color.white.opacity(0.8)), in: .rect(cornerRadius: 30.0))
                .padding(.bottom, 20)
                .padding(.horizontal, 30)
                
                if showErrorValues {
                    Text(valueErrorMessage)
                }
                
                
                
                // Passes container
                if showPasses {//showPasses {
                    HStack(alignment: .center, spacing: 20) {
                        
                        Image("passes")
                            .resizable()
                            .foregroundStyle(Color.ivoryMist)
                            .frame(width: 25, height: 25)
                        
                        Text("Passes").textCase(.uppercase)
                            .font(.eunomiaRegular)
                            .kerning(8)
                            .foregroundStyle(Color.ivoryMist)
                        
                    }
                    
                    
                    // Pass containers
                    if (passModel.passes.first != nil) {
                        
                        let aos_UTCDate = utcStringToDate.date(from: String(passModel.passes.first!.aos.split(separator: ".0")[0]))
                        let aos_localString = utcDateToCompleteLocalString.string(from: aos_UTCDate!)
                        let aos_localDate = df.date(from: aos_localString)
                        let los_UTCDate = utcStringToDate.date(from: String(passModel.passes.first!.los.split(separator: ".0")[0]))
                        let los_localString = utcDateToCompleteLocalString.string(from: los_UTCDate!)
                        let los_localDate = df.date(from: los_localString)
                        let timeToFirstAOS = aos_localDate?.timeIntervalSince(Date.now)
                        let printedTime = comp.string(from: timeToFirstAOS!)
                        let totalD = los_localDate?.timeIntervalSince(aos_localDate!)
                        let durMin = totalD! / 60.rounded(.towardZero)
                        let durSec = totalD?.truncatingRemainder(dividingBy: 60)
                        let d = String(format: "%dm %ds", Int(durMin), Int(durSec!))
                        
                        let mE = passModel.passes.first?.maxEl
                        
                        var qosFill: Int {
                            if (passModel.passes.first!.quality == "POOR") {
                                return 25
                            } else if (passModel.passes.first!.quality == "FAIR") {
                                return 50
                            } else if (passModel.passes.first!.quality == "GOOD") {
                                return 75
                            } else {
                                return 100
                            }
                        }
                        
                        RoundedRectangle(cornerRadius: 18)
                            .fill(Color.ivoryMist)
                            .overlay(
                                VStack {
                                    HStack {
                                        RoundedRectangle(cornerRadius: 30)
                                            .fill(Color.prussianBlue.opacity(0.2))
                                            .stroke(.gray)
                                            .overlay(
                                                Text("Next pass")
                                                    .font(.eunomiaRegular)
                                                    .scaleEffect(y: 0.7)
                                                    .foregroundStyle(Color.darkSlateGrey)
                                                    .kerning(1)
                                            )
                                            .frame(width: 105, height: 25)
                                        
                                        Spacer()
                                        
                                        Text("in \(String(describing: printedTime!))")
                                            .font(.firstPass)
                                            .foregroundStyle(Color.darkSlateGrey)
                                            .padding(.top, 5)
                                    }
                                    .frame(maxWidth: .infinity)
                                    .padding(.bottom, 6)
                                    .padding(.horizontal, 8)
                                    
                                    HStack (alignment: .center, spacing: 25){
                                        
                                        
                                        VStack {
                                            
                                            Text("AOS")
                                                .font(Font.fetchPass)
                                                .foregroundStyle(Color.prussianBlue)
                                                .kerning(2)
                                            Text("\(utcDateToLocalString.string(from: aos_UTCDate!))")
                                                .font(Font.passData)
                                                .kerning(0.5)
                                                .foregroundStyle(Color.prussianBlue)
                                        }
                                        
                                        VStack {
                                            Text("MAX EL")
                                                .font(Font.fetchPass)
                                                .foregroundStyle(Color.prussianBlue)
                                                .kerning(2)
                                            Text("\(mE!.prefix(5))")
                                                .font(Font.passData)
                                                .kerning(0.5)
                                                .foregroundStyle(Color.prussianBlue)
                                        }
                                        
                                        VStack {
                                            Text("LOS")
                                                .font(Font.fetchPass)
                                                .foregroundStyle(Color.prussianBlue)
                                                .kerning(2)
                                            Text("\(utcDateToLocalString.string(from: los_UTCDate!))")
                                                .font(Font.passData)
                                                .kerning(0.5)
                                                .foregroundStyle(Color.prussianBlue)
                                        }
                                    }
                                    .frame(maxWidth: .infinity)
                                    .padding(.bottom, 5)
                                    
                                    HStack {
                                        Text("Duration").textCase(.uppercase)
                                            .font(Font.fetchPass)
                                            .fontWeight(.heavy)
                                            .foregroundStyle(Color.prussianBlue)
                                            .kerning(2)
                                        
                                        Spacer()
                                        
                                        Text("\(d)")//"\(durMin)m \(durSec)s")
                                            .font(Font.fetchPass)
                                            .foregroundStyle(Color.prussianBlue)
                                            .kerning(2)
                                    }
                                    .padding(.horizontal, 15)
                                    .frame(maxWidth: .infinity, alignment: .leading)
                                    
                                    HStack {
                                        Text("QoS")
                                            .font(Font.fetchPass)
                                            .foregroundStyle(Color.prussianBlue)
                                            .kerning(2)
                                        
                                        Spacer()
                                        
                                        RoundedRectangle(cornerRadius: 10)
                                            .fill(.clear)
                                            .stroke(.gray.opacity(0.4))
                                            .frame(width: 200, height: 8)
                                            .overlay (alignment: .leading) {
                                                RoundedRectangle(cornerRadius: 10)
                                                    .fill(Color.sageGreen)
                                                    .frame(width: CGFloat(200*qosFill/100))
                                            }
                                        
                                            
                                    }
                                    .padding(.horizontal, 15)
                                    .frame(maxWidth: .infinity, alignment: .leading)
                                }
                                    .padding(.horizontal, 10)
                                    .padding(.vertical, 10)
                            )
                            .frame(maxWidth: .infinity)
                            .frame(height: 155)
                            .padding(.bottom, 7)
                            .padding(.horizontal, 15)
                    }
                    
                    let indexedPasses: [(offset: Int, element: Pass)] = Array(passModel.passes.dropFirst().enumerated())
                    
                    ForEach(indexedPasses, id: \.offset) { index, p in
                        
                        let aosDateUTC = utcStringToDate.date(from: String(p.aos.split(separator: ".0")[0]))
                        let losDateUTC = utcStringToDate.date(from: String(p.los.split(separator: ".0")[0]))
                        let aosStringDisplay = utcDateToLocalString.string(from: aosDateUTC!)
                        let losStringDisplay = utcDateToLocalString.string(from: losDateUTC!)
                        let aos_localString = utcDateToCompleteLocalString.string(from: aosDateUTC!)
                        let aos_localDate = df.date(from: aos_localString)
                        let los_localString = utcDateToCompleteLocalString.string(from: losDateUTC!)
                        let los_localDate = df.date(from: los_localString)
                        let todayBool = Calendar.current.isDateInToday(aos_localDate!)
                        let tomorrowBool = Calendar.current.isDateInTomorrow(aos_localDate!)
                        let when = todayBool ? "Today" : (tomorrowBool ? "Tomorrow" : "\(futurePass.string(from: aos_localDate!))")
                        let futurePassesAOS = utcDateToLocalString_hh_mm.string(from: aosDateUTC!)
                        let passMin = Int(los_localDate!.timeIntervalSince(aos_localDate!) / 60.rounded(.towardZero))
                        var colorQOS: Color {
                            if (p.quality == "POOR") {
                                return .qosPoor.opacity(0.7)
                            } else if (p.quality == "FAIR") {
                                return .qosFair.opacity(0.9)
                            } else if (p.quality == "GOOD") {
                                return .qosGood.opacity(0.8)
                            } else {
                                return .qosExcellent.opacity(0.65)
                            }
                        }
                        
                        RoundedRectangle(cornerRadius: 15)
                            .fill(Color.ivoryMist.opacity(1))
                            .overlay(
                                HStack {
                                    
                                    VStack(alignment: .leading, spacing: -3) {
                                        Text("\(when), \(futurePassesAOS)")
                                            .font(.eunomiaRegular)
                                            .scaleEffect(y: 0.8)
                                            .foregroundStyle(Color.darkSlateGrey)
                                        
                                        Text("\(passMin) min · max \(p.maxEl.prefix(2))°")
                                            .font(.durElev)
                                            .foregroundStyle(Color.darkSlateGrey)
                                            .opacity(0.8)
                                    }
                                    .frame(maxWidth: .infinity, alignment: .leading)
                                    
                                    Text("\(p.quality)")
                                        .font(.durElev)
                                        .foregroundStyle(Color.prussianBlue)
                                        .padding(.horizontal, 8)
                                        .padding(.vertical, 3)
                                        .glassEffect(.regular.tint(colorQOS), in: .capsule)
                                }
                                .frame(maxWidth: .infinity)
                                .padding(.vertical, 8)
                                .padding(.horizontal, 20)
                            )
                            .frame(maxWidth: .infinity)
                            .frame(height: 60)
                            .padding(.bottom, 3)
                            .padding(.horizontal, 15)
                    }
                }
            }
        }
    }
}
                  

#Preview {
    let mockModel = PassViewModel()
    let mockGt = GroundTrackViewModel()
    mockModel.passes = Pass.mockPasses
    mockGt.points = [
        "0": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 45.0, lon: -30.0),
        "1": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 46.2, lon: -25.5),
        "2": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 47.1, lon: -20.1)
    ]
//    mockGt.satName = "ISS (ZARYA)"
//    mockGt.norad = "25544"
    return GS(satellite: .constant("25544"), startTime: .constant(Date.now), endTime: .constant(Date.now.addingTimeInterval(86400)), inputType: .constant(InputOptions.tle), searchItem: .constant(String("")), passModel: mockModel, gtViewModel: .constant(mockGt))
}
