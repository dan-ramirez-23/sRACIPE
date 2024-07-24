rm(list=ls())
devtools::install(args="--verbose")

library(sRACIPE)#, lib.loc = "/Users/danramirez/localR/4.2.2-arm")

# Test on repressilator
topo <- data.frame(Source=c("A","A","B","C","D","D"), 
                   Target=c("B","C","A","D","B","C"), 
                   Type=c(2,1,2,2,1,2))
genes <- unique(c(topo$Source, topo$Target))
nGenes <- 4
nEdges <- 6


## Simulation Parameters
noise <- 0
printTime <- 0.2
stepSize <- 0.05
simTime <- 100

# initial conditions
ics <- rep(50, nGenes)



##### SIMULATION #####
racipe <- sracipeSimulate(topo, numModels = 10000, plots = FALSE, genIC = TRUE,
                              genParams = TRUE, integrate = TRUE, integrateStepSize = stepSize,
                              simulationTime = 20, initialNoise = noise, nNoise = 0, simDet = T, anneal = F)


# Save results
#saveRDS(racipe, file.path("tests","racipe_CTS_lambda_old.Rds"))
saveRDS(racipe, file.path("tests","racipe_CTS_lambda_new.Rds"))


# Plot comparison
racipe_old <- readRDS(file.path("tests","racipe_CTS_lambda_old.Rds"))
racipe_new <- readRDS(file.path("tests","racipe_CTS_lambda_new.Rds"))

racipe_old <- sracipeNormalize(racipe_old)
racipe_old <- sracipeNormalize(racipe_old)

assay_old <- assay(racipe_old)
assay_new <- assay(racipe_new)



plot(assay_old[1,], assay_old[2,])
plot(assay_new[1,], assay_new[2,])



hist(assay_old[2,])
hist(assay_new[2,])

hist(assay_old[1,])
hist(assay_new[1,])




### edit config
load("data/configData.rda")
configData$simParams[["signalGene"]] <- 0
configData$simParams[["maxFC"]] <- 1
save(configData, file = "data/configData.rda")
