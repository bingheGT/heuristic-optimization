library(lattice)
library(latticeExtra)

WD <- '~/Dropbox/FINAL/Kenzo_Clauw_Assignment2_Heuristic_Optimization/Results'
setwd(WD)

igs_50_20_01 <- read.csv("./runtime/igs_50_20_01.txt", header=TRUE, sep=",")
igs_50_20_02 <- read.csv("./runtime/igs_50_20_02.txt", header=TRUE, sep=",")
igs_50_20_03 <- read.csv("./runtime/igs_50_20_03.txt", header=TRUE, sep=",")
igs_50_20_04 <- read.csv("./runtime/igs_50_20_04.txt", header=TRUE, sep=",")
igs_50_20_05 <- read.csv("./runtime/igs_50_20_05.txt", header=TRUE, sep=",")

hga_50_20_01 <- read.csv("./runtime/hga_50_20_01.txt", header=TRUE, sep=",")
hga_50_20_02 <- read.csv("./runtime/hga_50_20_02.txt", header=TRUE, sep=",")
hga_50_20_03 <- read.csv("./runtime/hga_50_20_03.txt", header=TRUE, sep=",")
hga_50_20_04 <- read.csv("./runtime/hga_50_20_04.txt", header=TRUE, sep=",")
hga_50_20_05 <- read.csv("./runtime/hga_50_20_05.txt", header=TRUE, sep=",")

results50 <- read.csv("./50/results50.csv", header=TRUE, sep=",")
names(results50) <- c("instance","igs","hga")
results100 <- read.csv("./100/results100.csv", header=TRUE, sep=",")
names(results100) <- c("instance","igs","hga")
head(results50)
wilcox.test(results50$igs ,results50$hga, data=results50)
wilcox.test(results100$igs ,results100$hga, data=results100)

vals <- data.frame(igs = igs_50_20_01$relative,hga = hga_50_20_01$relative)
p <- ecdfplot(~ igs + hga, data=vals, auto.key=list(space='right'),xlab="Runtime",ylab="P(s)")
plot(p)

vals <- data.frame(igs = igs_50_20_02$relative,hga = hga_50_20_02$relative)
p <- ecdfplot(~ igs + hga, data=vals, auto.key=list(space='right'),xlab="Runtime",ylab="P(s)")
plot(p)

vals <- data.frame(igs = igs_50_20_03$relative,hga = hga_50_20_03$relative)
p <- ecdfplot(~ igs + hga, data=vals, auto.key=list(space='right'),xlab="Runtime",ylab="P(s)")
plot(p)

vals <- data.frame(igs = igs_50_20_04$relative,hga = hga_50_20_04$relative)
p <- ecdfplot(~ igs + hga, data=vals, auto.key=list(space='right'),xlab="Runtime",ylab="P(s)")
plot(p)

vals <- data.frame(igs = igs_50_20_05$relative,hga = hga_50_20_05$relative)
p <- ecdfplot(~ igs + hga, data=vals, auto.key=list(space='right'),xlab="Runtime",ylab="P(s)")
plot(p)

plot(p)
# scatterplot
g_range <- range(0, igs100$igs, genetic100$hga)
plot(igs100$igs, genetic100$hga, main="Scatterplot ARPD",xlab="Iterated Greed Search 100 ", ylab="Hybrid Genetic Algorithm 100", pch=19) 
abline(lm(genetic100$hga~igs100$igs), col="red") # regression line (y~x)

setwd(WD50)
results50 <- read.csv("./results50.csv", header=TRUE, sep=",")
igs50 <- results50[c(1,2)] 
genetic50 <- results50[c(1,3)]
tail(igs50)
# Calculate range from 0 to max value
g_range <- range(0, igs50$igs, genetic50$hga)
plot(igs50$IGS, genetic50$HGA, main="Scatterplot ARPD",xlab="Iterated Greed Search 50 ", ylab="Hybrid Genetic Algorithm 50", pch=19) 
abline(lm(genetic50$HGA~igs50$IGS), col="red") # regression line (y~x)




