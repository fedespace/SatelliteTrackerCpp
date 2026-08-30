//
//  Map3d.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI
import RealityKit
import MapKit

struct Map3d: View {
    @State private var radiusEarth: Float = 0.9
    @Binding var gtViewModel: GroundTrackViewModel
    @State private var detailsViewModel = SatelliteDetailsViewModel()
    @Binding var detailsSatellite: SatelliteDetails
    static let whiteMaterial = SimpleMaterial(color: .white, isMetallic: false)
    // Initial rotation angle
    @State private var baseRotation = simd_quatf(angle: 0, axis: SIMD3<Float>(0, 1, 0))
    @State private var currentRotation = simd_quatf(angle: 0, axis: SIMD3<Float>(0, 1, 0))
    @Binding var coord3dmap: [SIMD3<Float>]
    @Binding var startTime: Date
    @Binding var endTime: Date
    var norad: String {
        $gtViewModel.norad.wrappedValue
    }
    var incl: Double {detailsSatellite.incl}
    var apo: Int {detailsSatellite.apogee}
    var peri: Int {detailsSatellite.perigee}
    var period: Double {detailsSatellite.period}
    var orbitType: String {detailsSatellite.orbitType}
    var altitude: Int {(apo + peri)/2}
    var semimajorAxis: Double {Double(apo + peri)/2 + 6371}
    var meanSpeed: Double {2*Double.pi*semimajorAxis/period}
    var revsDay: Double {1440/period}
    var ecc: Double {
        var rEarth = 6371 // mean value [km]
        var ra = Double(detailsSatellite.apogee + rEarth)
        var rp = Double(detailsSatellite.perigee + rEarth)
        return Double(ra - rp) / Double(ra + rp)
    }
    
    // Time in the inputs
    var timeInput: DateFormatter {
        let timeInput = DateFormatter()
        timeInput.dateFormat = "HH:mm"
        return timeInput
    }
    // Future passes
    var futurePass: DateFormatter {
        let futurePass = DateFormatter()
        futurePass.dateFormat = "d MMM"
        return futurePass
    }
    
    var body: some View {
        
        ZStack {
            
            Color.yaleBlue.ignoresSafeArea()
            
            VStack(spacing: 15) {
                VStack (spacing: 0) {
                    HStack(spacing: 20) {
                        
                        Image("satellite")
                            .resizable()
                            .foregroundStyle(Color.ivoryMist)
                            .frame(width: 30, height: 30)
                        
                        //Text("gtViewModel.satName")
                        Text($gtViewModel.satName.wrappedValue)
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
                .padding(.top, 10)
                
                
                HStack {
                    // Type of orbit
                    RoundedRectangle(cornerRadius: 30)
                        .frame(width: 55, height: 28)
                        .glassEffect(.regular)
                        .overlay (
                            Text("\(orbitType)")
                                .font(.coord)
                                .foregroundStyle(Color.ivoryMist)
                        )
                    
                    // Altitude
                    RoundedRectangle(cornerRadius: 30)
                        .frame(width: 170, height: 28)
                        .overlay (
                            Text("\(altitude) km of altitude")
                                .font(.coord)
                                .foregroundStyle(Color.ivoryMist)
                        )
                        .glassEffect(.regular)
                    
                    // Ave velocity
                    RoundedRectangle(cornerRadius: 30)
                        .frame(width: 110, height: 28)
                        .glassEffect(.regular)
                        .overlay (
                            Text("\(meanSpeed) km/s")
                                .font(.coord)
                                .foregroundStyle(Color.ivoryMist)
                        )
                    
                }
                
                
                // Create the 3d sphere
                RealityView { content in
                    let mesh = MeshResource.generateSphere(radius: radiusEarth)
                    var material = PhysicallyBasedMaterial()
                    do {
                        let texture = try await TextureResource(named: "earth_map")
                        material.baseColor = PhysicallyBasedMaterial.BaseColor(texture: .init(texture))
                        material.roughness = 0.3
                    } catch {
                        print("Failed to load Earth texture: \(error)")
                        material.baseColor = PhysicallyBasedMaterial.BaseColor(tint: .blue)
                    }
                    let sphereEntity = ModelEntity(mesh: mesh, materials: [material])
                    sphereEntity.generateCollisionShapes(recursive: false)
                    sphereEntity.components.set(InputTargetComponent())
                    sphereEntity.name = "earth"
                    content.add(sphereEntity)
                    
                    for i in 0..<(coord3dmap.count - 1) {
                        let p1 = coord3dmap[i]
                        let p2 = coord3dmap[i+1]
                        
                        let mesh = MeshResource.generateCylinder(height: 1, radius: 0.005)
                        let entity = ModelEntity(mesh: mesh, materials: [SimpleMaterial(color: .red, isMetallic: false)])
                        
                        entity.scale.y = length(p2 - p1)
                        entity.position = (p1 + p2) / 2
                        entity.orientation = simd_quatf(from: SIMD3<Float>(0,1,0), to: normalize(p2 - p1))
                        
                        sphereEntity.addChild(entity)
                    }
                    
                } update: { content in
                    if let sphereEntity = content.entities.first(where: { $0.name == "earth" }) {
                        sphereEntity.transform.rotation = currentRotation
                    }
                }
                .gesture(
                    DragGesture()
                    // get the gesture on the entire sphere
                        .targetedToAnyEntity()
                        .onChanged { a in
                            let deltaX = Float(a.translation.width) / 200.0
                            let deltaY = Float(a.translation.height) / 200.0
                            let rotationX = simd_quatf(angle: deltaX, axis: SIMD3<Float>(0, 1, 0))
                            let rotationY = simd_quatf(angle: deltaY, axis: SIMD3<Float>(1, 0, 0))
                            currentRotation = rotationY * rotationX * baseRotation
                        }
                        .onEnded { a in
                            baseRotation = currentRotation
                        }
                )
                .padding(0)
                .frame(width: 500, height: 360)
                
                VStack (spacing: 10) {
                    Text("orbital details".uppercased())
                        .font(.gsTitle)
                        .foregroundStyle(Color.prussianBlue)
                    
                    HStack (spacing: 60) {
                        VStack (alignment: .leading, spacing: 10) {
                            VStack (alignment: .leading) {
                                Text("Inclination".uppercased())
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                                
                                Text("\(incl.formatted(.number.precision(.fractionLength(2))))".prefix(5))
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                            }
                            
                            VStack (alignment: .leading) {
                                Text("Apogee".uppercased())
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                                
                                Text("\(apo) km")
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                            }
                            
                            VStack (alignment: .leading) {
                                Text("Eccentricity".uppercased())
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                                
                                Text("\(ecc.formatted(.number.precision(.fractionLength(4))))")
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                            }
                        }
                        
                        VStack (alignment: .leading, spacing: 10) {
                            VStack (alignment: .leading) {
                                Text("Period".uppercased())
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                                
                                Text("\(period.formatted(.number.precision(.fractionLength(2)))) min")
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                            }
                            
                            VStack (alignment: .leading) {
                                Text("Perigee".uppercased())
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                                
                                Text("\(peri) km")
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                            }
                            
                            VStack (alignment: .leading) {
                                Text("Revs/day".uppercased())
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                                
                                Text("\(revsDay.formatted(.number.precision(.fractionLength(2))))")
                                    .font(.coord)
                                    .foregroundStyle(Color.prussianBlue)
                            }
                        }
                    }
                }
                .padding(.horizontal, 25)
                .padding(.vertical, 15)
                .glassEffect(.regular.tint(.prussianBlue.opacity(0.1)), in: .rect(cornerRadius: 20))
                .onChange(of: norad, { oldValue, newValue in
                    Task {
                        detailsSatellite = await detailsViewModel.fetchDetails(norad: newValue)
                    }
                })
                Spacer()
                
            }
            .frame(maxWidth: .infinity, alignment: .center)
            .padding(.bottom, 20)
            .padding(.top, 5)
            
            
            Spacer()
        }
    }
}

#Preview {
    let mockGt = GroundTrackViewModel()
    let satelliteDetails = SatelliteDetails(
        objectName: "SKYLENS-DEMO 1",
        objectType: "PAYLOAD",
        noradID: 43205,
        opsStatusCode: "+",
        owner: "US",
        launched: "2018-02-22",
        launchSite: "VAFB",
        decay: "",
        period: 94.6,
        incl: 97.4,
        apogee: 575,
        perigee: 560,
        rcs: 1.8,
        dataStatusCode: "",
        orbitCenter: "EA",
        orbitType: "SSO"
    )
    mockGt.points = [
        "0": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 45.0, lon: -30.0, alt: 1200),
        "1": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 46.2, lon: -25.5, alt: 1200),
        "2": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 47.1, lon: -20.1, alt: 1200)
    ]
    return Map3d(gtViewModel: .constant(mockGt), detailsSatellite: .constant(satelliteDetails), coord3dmap: .constant(mockGt.coord3dmap), startTime: .constant(Date.now), endTime: .constant(Date.now.addingTimeInterval(86400)))
}
