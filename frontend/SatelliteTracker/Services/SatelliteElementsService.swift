//
//  SatelliteElementsService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/06/2026.
//

import SwiftUI

struct SatelliteElementsService {
    
    func fetchElements() async throws -> String {
        let request = URLRequest(url: URL(string: "https://www.ucs.org/sites/default/files/2022-02/UCS-Satellite-Database-1-1-2022.txt")!)
        let (data, _) = try await URLSession.shared.data(for:request)
        let stringUCL = String(data: data, encoding: .utf8) ?? "nil"
        print(stringUCL)
        return stringUCL
    }
    
}
