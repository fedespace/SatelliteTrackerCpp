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
    @State var gsLat: String = ""
    @State var gsLon: String = ""
    @State var gsAlt: String = ""
    @State var gsMask: String = ""

    
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
                            .padding(.top, 5)
                    }
                    .padding(.top, 12)
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
                    .padding(.bottom, 15)
                    .frame(maxWidth: .infinity)
                    
                }
                .padding(.leading, 25)
                .frame(maxWidth: .infinity, alignment: .leading)
                .glassEffect(.regular.tint(.darkSlateGrey), in: .rect(cornerRadius: 30.0))
                
                
                GlassEffectContainer {
                    
                    HStack(spacing: 10) {
                        
                        Image("gs")
                            .resizable()
                            .foregroundStyle(Color.darkSlateGrey)
                            .frame(width: 30, height: 30)
                        
                        Text("Ground Station").textCase(.uppercase)
                            .font(Font.gsTitle)
                            .fontWeight(.bold)
                            .foregroundStyle(Color.darkSlateGrey)
                            .padding(.top, 5)
                        
                    }
                    .padding(.top, 12)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    
                    
                    HStack(alignment: .center, spacing: 15) {
                        
                        VStack {
                            
                            VStack(spacing: 0) {
                                Text("Latitude [deg]")
                                    .font(.coord)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsLat)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.6)), in: .rect(cornerRadius: 10))
                            }
                            
                            
                            VStack(spacing: 0) {
                                Text("Altitude [m]")
                                    .font(.coord)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsAlt)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.6)), in: .rect(cornerRadius: 10))
                            }
                            
                        }
                        .frame(maxWidth: .infinity)
                        
                        VStack {
                            
                            VStack(spacing: 0) {
                                Text("Longitude [deg]")
                                    .font(.coord)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsLon)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.6)), in: .rect(cornerRadius: 10))
                            }
                            
                            
                            VStack(spacing: 0) {
                                Text("Elev. Mask [deg]")
                                    .font(.coord)
                                    .foregroundStyle(Color.darkSlateGrey)
                                TextField("", text: $gsMask)
                                    .frame(maxWidth: 120)
                                    .glassEffect(.regular.tint(.darkSlateGrey.opacity(0.6)), in: .rect(cornerRadius: 10))
                            }
                            
                        }
                        .frame(maxWidth: .infinity)
                        
                        
                        
                    }
                    .padding(.bottom, 25)
                    .frame(maxWidth: .infinity, alignment: .center)
                    
                    
                    
                }
                .padding(.horizontal, 20)
                .frame(maxWidth: .infinity, alignment: .leading)
                .glassEffect(.regular, in: .rect(cornerRadius: 30.0))
                .padding(.top, 10)
                    
                    
                
                
                
                
            }
            .padding(.horizontal, 30)
            .padding(.top, 20)
            
        }
    }
}


#Preview {
    GS(satellite: .constant("ISS (ZARYA)"), startTime: .constant(Date.now), endTime: .constant(Date.now))
}
