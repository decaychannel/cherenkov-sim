# Cherenkov Radiation Simulation

Geant4 simulation demonstrating Cherenkov radiation in water.

## Physics

When a charged particle travels through a medium faster than the speed of light in that medium, it emits **Cherenkov radiation** - a cone of blue light. This simulation demonstrates this phenomenon using:

- **Radiator**: 50×50×50 cm water cube (n ≈ 1.33)
- **Particle**: 500 MeV μ⁻ (muon)
- **Result**: Characteristic Cherenkov cone of optical photons

## Project Structure

```
cherenkov-sim/
├── CMakeLists.txt          # Build configuration
├── main.cc                 # Main program with optical physics
├── README.md               # This file
├── include/
│   ├── DetectorConstruction.hh
│   ├── Materials.hh
│   ├── ActionInitialization.hh
│   └── PrimaryGeneratorAction.hh
├── src/
│   ├── DetectorConstruction.cc  # Water cube geometry
│   ├── Materials.cc             # Optical properties (refractive index)
│   ├── ActionInitialization.cc
│   └── PrimaryGeneratorAction.cc # Muon beam
└── macros/
    ├── vis.mac             # Visualization with trajectory filtering
    └── run.mac             # Batch run macro
```

## Requirements

- Geant4 11.x (with OpenGL visualization)
- CMake 3.16+
- C++ compiler with C++17 support

## Building

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Running

### Interactive Mode (with visualization)
```bash
./cherenkov
```

Then in the Geant4 session:
```
/run/beamOn 1
```

### Batch Mode
```bash
./cherenkov macros/run.mac
```

## Visualization

| Color   | Particle          |
|---------|-------------------|
| Magenta | Muon (μ⁻)         |
| Yellow  | Cherenkov photons |
| Green   | Gamma rays        |
| Red     | Electrons         |
| Blue    | Positrons         |

The trajectory filter shows only particles with Parent ID = 0 or 1, meaning:
- Primary muon
- Direct Cherenkov photons from the muon

## Customization

### Change Particle Energy
Edit `src/PrimaryGeneratorAction.cc`:
```cpp
fParticleGun->SetParticleEnergy(1.0*GeV);  // Higher energy = more photons
```

### Change Radiator Size
Edit `src/DetectorConstruction.cc`:
```cpp
auto waterSolid = new G4Box("WaterTank", 50*cm, 50*cm, 50*cm);  // Larger tank
```

### Disable Trajectory Filter
In interactive mode:
```
/vis/filtering/trajectories/attributeFilter-0/active false
```

## License

Free to use for academic and research purposes.
