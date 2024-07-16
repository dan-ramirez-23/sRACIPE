rm(list=ls())
devtools::install(args="--verbose")

library(sRACIPE)#, lib.loc = "/Users/danramirez/localR/4.2.2-arm")

# Test on repressilator
topo <- data.frame(Source=c("A"), Target=c("B"), Type=c(2))
nGenes <- 2
nEdges <- 1
genes <- c("A","B")


## Simulation Parameters
noise <- 0.2
printTime <- 0.2
stepSize <- 0.01
simTime <- 100

# initial conditions
ics <- rep(50, nGenes)


## Model Parameters
g <- 30
k <- 0.5
n <- 4
fc <- 50
th <- 20



##### SIMULATION #####
racipe <- sracipeSimulate(topo, numModels = 100, plots = FALSE, genIC = TRUE,
                              genParams = TRUE, integrate = TRUE, integrateStepSize = stepSize,
                              simulationTime = 20, initialNoise = noise, nNoise = 1, simDet = F, anneal = F)


# Save results
saveRDS(racipe, file.path("tests","racipe_CTS_lambda_old.Rds"))



