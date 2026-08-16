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
    @State var gsLat: String = ""
    @State var gsLon: String = ""
    @State var gsAlt: String = ""
    @State var gsMask: String = ""
    @State var passModel = PassViewModel()
    @State var passes: [Pass] = []
    @State var aosDate = Date()
    @State var losDate = Date()
    @State var maxElevation: String = ""
    @State var duration: String = ""
    
    
    var body: some View {
        
        
        
        ZStack {
            
            Color.yaleBlue.ignoresSafeArea()
            
            ScrollView {
                
                GlassEffectContainer {
                    
                    HStack(spacing: 10) {
                        
                        Image("satellite")
                            .resizable()
                            .foregroundStyle(Color.ivoryMist)
                            .frame(width: 30, height: 30)
                            .padding(0)
                        
                        
                        Text("\($satellite.wrappedValue)")
                            .font(Font.gsTitle)
                            .fontWeight(.bold)
                            .foregroundStyle(Color.ivoryMist)
                    }
                    .frame(maxWidth: .infinity, alignment: .leading)
                    
                    VStack (alignment: .leading) {
                        
                        Text("from \($startTime.wrappedValue.formatted(date: .abbreviated, time: .shortened))")
                            .font(Font.gsWindow)
                            .kerning(2)
                            .foregroundStyle(Color.ivoryMist)
                            .frame(maxWidth: .infinity, alignment: .leading)
                            .padding(.bottom, -15)
                        
                        Text("to \($endTime.wrappedValue.formatted(date: .abbreviated, time: .shortened))")
                            .font(Font.gsWindow)
                            .kerning(2)
                            .foregroundStyle(Color.ivoryMist)
                            .frame(maxWidth: .infinity, alignment: .leading)
                        
                    }
                    .frame(maxWidth: .infinity)
                    
                }
                .padding(20)
                .frame(maxWidth: .infinity, alignment: .leading)
                .glassEffect(.regular.tint(.darkSlateGrey), in: .rect(cornerRadius: 30.0))
                
                
                GlassEffectContainer {
                    
                    HStack(spacing: 10) {
                        
                        Image("gs")
                            .resizable()
                            .foregroundStyle(Color.darkSlateGrey)
                            .frame(width: 30, height: 30)
                            .padding(.bottom, 8)
                        
                        Text("Ground Station").textCase(.uppercase)
                            .font(Font.gsTitle)
                            .fontWeight(.bold)
                            .foregroundStyle(Color.darkSlateGrey)
                        
                    }
                    .frame(maxWidth: .infinity, alignment: .leading)
                    
                    
                    HStack(alignment: .center, spacing: 15) {
                        
                        VStack {
                            
                            VStack(spacing: 0) {
                                Text("Latitude [deg]")
                                    .fontWeight(.semibold)
                                    .font(.coord)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsLat)
                                    .multilineTextAlignment(.center)
                                    .padding(.vertical, 2)
                                    .padding(.horizontal, 15)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.3)), in: .rect(cornerRadius: 10))
                            }
                            
                            
                            VStack(spacing: 0) {
                                Text("Altitude [m]")
                                    .font(.coord)
                                    .fontWeight(.semibold)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsAlt)
                                    .multilineTextAlignment(.center)
                                    .padding(.vertical, 2)
                                    .padding(.horizontal, 15)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.3)), in: .rect(cornerRadius: 10))
                            }
                            
                        }
                        .frame(maxWidth: .infinity)
                        
                        VStack {
                            
                            VStack(spacing: 0) {
                                Text("Longitude [deg]")
                                    .font(.coord)
                                    .fontWeight(.semibold)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsLon)
                                    .multilineTextAlignment(.center)
                                    .padding(.vertical, 2)
                                    .padding(.horizontal, 15)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.3)), in: .rect(cornerRadius: 10))
                            }
                            
                            
                            VStack(spacing: 0) {
                                Text("Elev. Mask [deg]")
                                    .font(.coord)
                                    .fontWeight(.semibold)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsMask)
                                    .multilineTextAlignment(.center)
                                    .padding(.vertical, 2)
                                    .padding(.horizontal, 15)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.3)), in: .rect(cornerRadius: 10))
                            }
                            
                        }
                        .frame(maxWidth: .infinity)
                        
                        
                    }
                    .frame(maxWidth: .infinity, alignment: .center)
                    
                    // Button to fetch passes
                    HStack {
                        
                        Text("Fetch")
                            .textCase(.uppercase)
                            .foregroundStyle(Color.ivoryMist)
                            .font(.fetchPass)
                        
                        Image(systemName: "arrow.forward")
                            .foregroundStyle(Color.ivoryMist)
                        
                    }
                    .padding(.horizontal, 15)
                    .padding(.vertical, 5)
                    .glassEffect(.regular.tint(Color.darkSlateGrey), in: .rect(cornerRadius: 30))
                    .padding(.top, 5)
                    .onTapGesture {
                        passes = []
                        Task {
                            await passModel.fetchPasses(satellite: satellite, startTime: startTime, endTime: endTime, gsLat: gsLat, gsLon: gsLon, gsAlt: gsAlt, gsMask: gsMask, inputType: inputType, searchItem: searchItem!)
                        }
                    }
                    
                }
                .padding(.horizontal, 20)
                .padding(.top, 20)
                .padding(.bottom, 15)
                .frame(maxWidth: .infinity, alignment: .leading)
                .glassEffect(.regular.tint(Color.white), in: .rect(cornerRadius: 30.0))
                .padding(.vertical, 10)
                
                // Passes container
                GlassEffectContainer (spacing: 20) {
                    
                    HStack(spacing: 10) {
                        
                        Image("passes")
                            .resizable()
                            .foregroundStyle(Color.ivoryMist)
                            .frame(width: 25, height: 25)
                        
                        Text("Passes").textCase(.uppercase)
                            .font(Font.gsTitle)
                            .fontWeight(.bold)
                            .foregroundStyle(Color.ivoryMist)
                            .padding(.top, 5)
                        
                        Spacer()
                        
                    }
                    
                    
                    // Pass container
                    let indexedPasses: [(offset: Int, element: Pass)] = Array(passModel.passes.enumerated())
                    
                    ForEach(indexedPasses, id: \.offset) { index, p in
                        
                        VStack {
                            
                            HStack (alignment: .center, spacing: 20){
                                
                                
                                VStack {
                                    
                                    
                                    Text("AOS")
                                        .font(Font.fetchPass)
                                        .foregroundStyle(Color.prussianBlue)
                                        .kerning(2)
                                    Text("\(passModel.aosLocal[index])")
                                        .font(Font.fetchPass)
                                        .foregroundStyle(Color.prussianBlue)
                                }
                                
                                VStack {
                                    Text("LOS")
                                        .font(Font.fetchPass)
                                        .foregroundStyle(Color.prussianBlue)
                                        .kerning(2)
                                    Text("\(passModel.losLocal[index])")
                                        .font(Font.fetchPass)
                                        .foregroundStyle(Color.prussianBlue)
                                }
                                
                                VStack {
                                    Text("MAX EL")
                                        .font(Font.fetchPass)
                                        .foregroundStyle(Color.prussianBlue)
                                        .kerning(2)
                                    Text("\(p.maxEl.prefix(5))")
                                        .font(Font.fetchPass)
                                        .foregroundStyle(Color.prussianBlue)
                                }
                                
                                
                            }
                            .padding(.bottom, 5)
                            
                            // Duration
                            HStack {
                                Text("Duration").textCase(.uppercase)
                                    .font(Font.fetchPass)
                                    .fontWeight(.heavy)
                                    .foregroundStyle(Color.prussianBlue)
                                    .kerning(2)
                                
                                Spacer()
                                
                                Text("\(passModel.duration[index])")
                                    .font(Font.fetchPass)
                                    .foregroundStyle(Color.prussianBlue)
                                    .kerning(2)
                            }
                            .padding(.horizontal, 15)
                            .frame(maxWidth: .infinity, alignment: .leading)
                            
                            
                            // Quality of Signal
                            HStack {
                                Text("Quality").textCase(.uppercase)
                                    .font(Font.fetchPass)
                                    .foregroundStyle(Color.prussianBlue)
                                    .kerning(2)
                                
                                Spacer()
                                
                                Text("percent bar") //da costruire
                            }
                            .padding(.horizontal, 15)
                            .frame(maxWidth: .infinity, alignment: .leading)
                            
                        }
                        .padding(15)
                        .frame(maxWidth: .infinity)
                        .glassEffect(.regular.tint(Color.white), in: .rect(cornerRadius: 18))
                        .padding(.bottom, 8)
                        
                    }
                }
                .padding(.top, 20)
                .padding(.bottom, 15)
                .padding(.horizontal, 20)
                .frame(maxHeight: .infinity)
                .frame(maxWidth: .infinity)
                .glassEffect(.clear, in: .rect(cornerRadius: 30))
                
            }
            .padding(.horizontal, 30)
            .padding(.top, 20)
        }
    }
}

#Preview {
    GS(satellite: .constant("25544"), startTime: .constant(Date.now), endTime: .constant(Date.now.addingTimeInterval(86400)), inputType: .constant(InputOptions.tle), searchItem: .constant(String("")))
}
