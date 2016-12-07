# CorrelationCountermeasure
Increasing anonymity of anonymous networks by protecting against flow correlation attacks

#Techniques:
The two techniques we implemented as elements of click are:
* Packet Padding : Adding random bytes to packets to obfuscate the src and dst flows
* Cover Traffic : Sending a copy of the packet to the lowest avg flow destination to cover where the traffic is actually meant for

# Analysis (Spearman Rank Correlation Coefficient)
We use a python script that takes data and calculates the Spearman Rank Correlation Coefficient. The scale at which we are evaluating the coefficient is as follows:
* 0.00 -> 0.19 : "Very Weak"
* 0.20 -> 0.39 : "Weak"
* 0.40 -> 0.59 : "Moderate"
* 0.60 -> 0.79 : "Strong"
* 0.80 -> 1.00 : "Very Strong"

