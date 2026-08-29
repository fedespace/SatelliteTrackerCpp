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
    @State private var radiusEarth: Float = 0.45
    @Binding var gtViewModel: GroundTrackViewModel
    static let whiteMaterial = SimpleMaterial(color: .white, isMetallic: false)
    // Initial rotation angle
    @State private var baseRotation = simd_quatf(angle: 0, axis: SIMD3<Float>(0, 1, 0))
    @State private var currentRotation = simd_quatf(angle: 0, axis: SIMD3<Float>(0, 1, 0))
    var curvedGt: MeshDescriptor{
        var a = MeshDescriptor(name: "groundtrack")
        a.positions = MeshBuffer(coord3dmap)
        a.primitives = .triangles([0, 1, 2])
        return a
    }
    @Binding var coord3dmap: [SIMD3<Float>]
    @Binding var startTime: Date
    @Binding var endTime: Date
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
            
            VStack(spacing: 10) {
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
    mockGt.points = [
        "0": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 45.0, lon: -30.0),
        "1": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 46.2, lon: -25.5),
        "2": GroundTrackPoint(line1: "1 25544U 98067A...", line2: "2 25544  51.6400...", name: "ISS (ZARYA)", norad: "25544", lat: 47.1, lon: -20.1)
    ]
    return Map3d(gtViewModel: .constant(mockGt), coord3dmap: .constant(mockGt.coord3dmap), startTime: .constant(Date.now), endTime: .constant(Date.now.addingTimeInterval(86400)))
}
