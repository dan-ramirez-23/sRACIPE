# sRACIPE 

*Stochastic analysis for Random Circuit Perturbation*



sRACIPE implements a randomization-based method for gene circuit modeling. It allows us to study the effect of both the gene expression noise and the parametric variation on any gene regulatory circuit (GRC) using only its topology, and simulates an ensemble of models with random kinetic parameters at multiple noise levels. Statistical analysis of the generated gene expressions reveals the basin of attraction and stability of various phenotypic states and their changes associated with intrinsic and extrinsic noises. sRACIPE provides a holistic picture to evaluate the effects of both the stochastic nature of cellular processes and the parametric variation.   

If you use sRACIPE, please consider citing our paper [Role of noise and parametric variation in the dynamics of gene regulatory circuits](https://www.nature.com/articles/s41540-018-0076-x) published in [npj Systems Biology and Applications](https://www.nature.com/npjsba/articles).

*Installation*

    if (!requireNamespace("BiocManager", quietly = TRUE))
    install.packages("BiocManager")
    BiocManager::install("sRACIPE")

Or install the github version for new features. This will allow installation of 
most recent sRACIPE even if you are using older R and Bioconductor.

    library(devtools)
    install_github(“vivekkohar/sRACIPE”)

Build Reports
------

- [![Bioconductor Release](https://bioconductor.org/shields/build/release/bioc/sRACIPE.svg)](https://bioconductor.org/checkResults/release/bioc-LATEST/sRACIPE/) – Bioconductor Release
- [![Bioconductor devel](https://bioconductor.org/shields/build/devel/bioc/sRACIPE.svg)](https://bioconductor.org/checkResults/devel/bioc-LATEST/sRACIPE/) – Bioconductor devel