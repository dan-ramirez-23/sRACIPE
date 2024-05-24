
rm(list=ls())


# library(doFuture)
# library(Rcpp)
# ### Install
.libPaths("/Users/danramirez/localR/4.2.2-arm")
.libPaths()
devtools::install()
devtools::install_github("dan-ramirez-23/sRACIPE")


devtools::build()
devtools::check()




#data("EMT1")
#EMT1 <- as.data.frame(EMT1)

topo <- read.table(file.path(getwd(),"../../","emt_state_transitions","EMTDriveR","inputs","emt_ffctopo_72node_5k200ic.tpo"), header = T)
EMT1 <- topo

# Recording the total elapsed time here cause that's what I care about sue me



## First, look @ integration step
# 100 models, 0.01, 20 time - 19s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 20, integrateStepSize = 0.01)})
# 100 models, 0.02, 20 time - 9.5s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 20, integrateStepSize = 0.02)})
# 100 models, 0.05, 20 time - 4s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 100, simulationTime = 20, integrateStepSize = 0.05)})






# 2000 models, 20 timesteps of 0.2 - EMT1
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 8, integrateStepSize = 0.2)})
# 8-core: 39s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 4, integrateStepSize = 0.2)})
# 4-core: 24s
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, integrateStepSize = 0.2)})
# single-threaded: 21s



# 2000 models, 20 timesteps of 0.2 - 72-node EMT circuit
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 20, nCores = 8, integrateStepSize = 0.2)})
# 8-core: 80s, 54% efficiency
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 20, nCores = 4, integrateStepSize = 0.2)})
# 4-core: 104s, 83% efficiency
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 20, integrateStepSize = 0.2)})
# single-threaded: 347s



# 2000 models, 50 timesteps of 0.2 - 72-node EMT circuit
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 50, nCores = 8, integrateStepSize = 0.2)})
# 8-core: 138s, 78% efficiency
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 50, nCores = 4, integrateStepSize = 0.2)})
# 4-core: 212s, 102% efficiency
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 50, integrateStepSize = 0.2)})
# single-threaded: 870s




## How about with noise?
# 2000 models, 20 timesteps of 0.2, 1 noise level - EMT1
#system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 8, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2)})
# 8-core: 
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 4, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet = F)})
# 4-core: 24s - 12% efficiency????
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet=F)})
# single-threaded: 12s?????




# 2000 models, 100 timesteps of 0.2, 1 noise level - EMT1
#system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 8, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2)})
# 8-core: 
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 100, nCores = 4, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet = F)})
# 4-core: 33s, 41% eff
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 100, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet=F)})
# single-threaded: 54s







## Now the large circuit with noise
# 2000 models, 20 timesteps of 0.2, 1 noise level - EMT1
#system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 8, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2)})
# 8-core: 
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 20, nCores = 4, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet = F)})
# 4-core: 64s - 73%
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 20, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet=F)})
# single-threaded: 188s




# 2000 models, 100 timesteps of 0.2, 1 noise level - EMT1
#system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 8, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2)})
# 8-core: 
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 100, nCores = 4, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet = F)})
# 4-core: 214s - 105%
system.time({racipe <- sracipeSimulate(circuit=topo, numModels = 2000, simulationTime = 100, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet=F)})
# single-threaded: 900s




## Does noise work with cores? 0.2 vs 2, same otherwise, 4 cores each
system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 4, integrateStepSize = 0.2, nNoise = 1, iNoise=0.2, simDet = F)})
assay(racipe)[1:5,1:5]

system.time({racipe <- sracipeSimulate(circuit=EMT1, numModels = 2000, simulationTime = 20, nCores = 4, integrateStepSize = 0.2, nNoise = 1, iNoise=2, simDet = F)})
assay(racipe)[1:5,1:5]





