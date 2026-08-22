//
//  PassViewModel.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 02/08/2026.
//

import SwiftUI


@Observable
class PassViewModel {
    var passes: [Pass] = []
    var utcFormatter: DateFormatter {
        let utcFormatter = DateFormatter()
        utcFormatter.dateFormat = "yyyy-M-d H:m:s"
        utcFormatter.timeZone = TimeZone(identifier: "UTC")
        utcFormatter.locale = Locale(identifier: "en_US_POSIX")
        return utcFormatter
    }
    var localFormatter: DateFormatter {
        let localFormatter = DateFormatter()
        localFormatter.dateFormat = "dd-MM-yyyy HH:mm:ss"
        localFormatter.timeZone = TimeZone.current
        return localFormatter
    }
    var durationFormatter: DateFormatter {
        let durationFormatter = DateFormatter()
        durationFormatter.dateFormat = "dd-MM-yyyy HH:mm:ss"
        return durationFormatter
    }
    var duration: [String] {
        passes.compactMap { p in
            let totalSeconds = Int(Double(p.duration)! * 60)
            let min = totalSeconds / 60
            let sec = totalSeconds % 60
            return String(format: "%dm %ds", min, sec)
            }
    }
    var inputType: InputOptions? = .name
    var searchItem: String? = ""
    
    
    func fetchPasses(name: String, line1: String, line2: String, startTime: Date, endTime: Date, gsLat: String, gsLon: String, gsAlt: String, gsMask: String, inputType: InputOptions, searchItem: String) async {
        let inputBinding = Binding<InputOptions?>(
                    get: { self.inputType },
                    set: { self.inputType = $0 }
                )
                
                let searchBinding = Binding<String?>(
                    get: { self.searchItem },
                    set: { self.searchItem = $0 }
                )
        let pService = PassService(inputType: inputBinding, searchItem: searchBinding)
        do {
            passes = try await pService.fetchPasses(name: name, line1: line1, line2: line2, startTime: startTime, endTime: endTime, gsLat: gsLat, gsLon: gsLon, gsAlt: gsAlt, gsMask: gsMask, inputType: inputType, searchItem: searchItem)
        } catch {
            let errorMess = error.localizedDescription
            print("Error message: \(errorMess)")
        }
    }
}
