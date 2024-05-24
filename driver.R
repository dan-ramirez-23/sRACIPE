
rm(list=ls())
data("EMT1")
EMT1 <- as.data.frame(EMT1)

#topo <- read.table(file.path(getwd(),"../../","emt_state_transitions","EMTDriveR","inputs","emt_ffctopo_72node_5k200ic.tpo"), header = T)


# Recording the total elapsed time here cause that's what I care about sue me

# 100 models, 10 timesteps
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 10)})
# single-threaded: 5s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 10, nCores = 4)})
# 4-core: 20s


# 100 models, 100 timesteps
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 100)})
# single-threaded: 45s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 100, nCores = 4)})
# 4-core: 30s



# 1000 models, 10 timesteps
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 1000, simulationTime = 10)})
# single-threaded: 46s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 1000, simulationTime = 10, nCores = 4)})
# 4-core: 31s


# 2000 models, 50 timesteps
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 50, integrateStepSize = 0.01)})
# single-threaded: 920
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 50, nCores = 4, integrateStepSize = 0.01)})
# 4-core: 239
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 50, nCores = 8, integrateStepSize = 0.01)})
# 8-core: 149



# 5000 models, 50 timesteps
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 5000, simulationTime = 50)})
# single-threaded: 1137
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 5000, simulationTime = 50, nCores = 4)})
# 4-core: 298
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 5000, simulationTime = 50, nCores = 8)})
# 8-core: 249



# 10000 models, 100 timesteps
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 10000, simulationTime = 100)})
# single-threaded: 
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 10000, simulationTime = 100, nCores = 4)})
# 4-core: 
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 10000, simulationTime = 100, nCores = 8)})
# 8-core: 
