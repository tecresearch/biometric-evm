import React, { useState } from "react";
import "./App.css";
import { ToastContainer, toast } from "react-toastify";
import "react-toastify/dist/ReactToastify.css";

function App() {
  const VOTER_LIMIT = 5; // Set the maximum number of voters
  const [fingerprintID, setFingerprintID] = useState(null);
  const [message, setMessage] = useState("Press Start to begin.");
  const [ledStatus, setLedStatus] = useState([false, false, false]);
  const [voteCounts, setVoteCounts] = useState([0, 0, 0]);
  const [votedIDs, setVotedIDs] = useState([]);
  const [winner, setWinner] = useState(null);
  const [totalVotes, setTotalVotes] = useState(0);
  const [step, setStep] = useState(0); // Track the current step
  const [isProcessing, setIsProcessing] = useState(false); // Track if the process is ongoing

  // Function to generate a random Voter ID
  const generateRandomVoterID = () => {
    return Math.floor(Math.random() * 10000);
  };

  // Function to simulate fingerprint authentication
  const handleFingerprintSubmit = () => {
    if (winner) {
      setMessage("Voting ended. Winner has been declared.");
      toast.info("Voting ended. Winner has been declared.");
      return;
    }

    setIsProcessing(true); // Start the process

    setMessage("Capturing...");
    toast.info("Capturing fingerprint...");

    // Simulate delay for capturing
    setTimeout(() => {
      setMessage("Generating Voter ID...");
      toast.info("Generating Voter ID...");

      // Simulate delay for generating ID
      setTimeout(() => {
        const generatedID = generateRandomVoterID();
        setFingerprintID(generatedID);
        setMessage(`Saving Voter ID: ${generatedID}...`);
        toast.success("Fingerprint recognized and Voter ID saved!");

        // Simulate saving the Voter ID
        setTimeout(() => {
          setMessage("Fingerprint recognized! You are authenticated.");
          setIsProcessing(false);
          setStep(2); // Move to next step (Authentication complete)
        }, 1000); // Simulate saving Voter ID delay
      }, 2000); // Simulate delay for generating ID
    }, 2000); // Simulate delay for capturing
  };

  // Handle Voting
  const handleVote = (partyIndex) => {
    if (totalVotes >= VOTER_LIMIT) {
      setMessage("Voting limit reached. No more votes allowed.");
      toast.info("All voters have cast their votes.");
      calculateWinner();
      return;
    }

    if (!fingerprintID) {
      setMessage("Please authenticate with your fingerprint first.");
      toast.warn("Authenticate with your fingerprint first.");
      return;
    }

    if (votedIDs.includes(fingerprintID)) {
      setMessage("You have already voted!");
      flashAllLEDs();
      toast.error("You have already voted!");
      return;
    }

    const newVoteCounts = [...voteCounts];
    newVoteCounts[partyIndex]++;
    setVoteCounts(newVoteCounts);
    setVotedIDs([...votedIDs, fingerprintID]);
    setTotalVotes(totalVotes + 1);

    setMessage(`Thanks for voting for Party ${partyIndex + 1}!`);
    toast.success(`Vote cast for Party ${partyIndex + 1}!`);
    toggleLED(partyIndex);

    if (totalVotes + 1 === VOTER_LIMIT) {
      calculateWinner();
    }
  };

  const calculateWinner = () => {
    const maxVotes = Math.max(...voteCounts);
    const leadingParties = voteCounts
      .map((count, index) => (count === maxVotes ? index + 1 : null))
      .filter((party) => party !== null);

    if (leadingParties.length === 1) {
      setWinner(`Party ${leadingParties[0]} wins the election!`);
      toast.success(`Party ${leadingParties[0]} wins the election!`);
    } else {
      setWinner(`It's a tie between Party ${leadingParties.join(" and ")}.`);
      toast.info("It's a tie!");
    }
  };

  const toggleLED = (index) => {
    setLedStatus((prev) => {
      const newStatus = [false, false, false];
      newStatus[index] = true;
      return newStatus;
    });
    setTimeout(() => setLedStatus([false, false, false]), 1000);
  };

  const flashAllLEDs = () => {
    setLedStatus([true, true, true]);
    setTimeout(() => setLedStatus([false, false, false]), 1000);
  };

  const resetSystem = () => {
    setFingerprintID(null);
    setMessage("Press Start to begin.");
    setLedStatus([false, false, false]);
    setVoteCounts([0, 0, 0]);
    setVotedIDs([]);
    setWinner(null);
    setTotalVotes(0);
    setStep(0);
    setIsProcessing(false);
    toast.info("System reset successfully.");
  };

  const handleStartVoting = () => {
    setMessage("Authentication complete. You can now vote.");
    setStep(3); // Move to voting step
  };

  return (<>
    <h1  className="App" >E-Voting: Embeddes Machine</h1><hr/>
    <div className="App">
     

      {/* Buttons for Procedure */}
      <div className="procedure-buttons">
        <button
          onClick={handleFingerprintSubmit} // Automatically generate fingerprint ID and authenticate
          className="procedure-btn"
         
        >
          Place Fingerprint
        </button>
        <button
          onClick={handleStartVoting}
          className="procedure-btn"
          disabled={step < 2 || isProcessing}
        >
          Start Voting
        </button>
        <button
          onClick={() => setMessage("Check leads and final results.")}
          className="procedure-btn"
          disabled={step < 3}
        >
          Show Leads & Winner
        </button>
        
{/* Voting Buttons */}
{step >= 3 && (
  <div className="vote-section">
    <button
      onClick={() => handleVote(0)}
      disabled={totalVotes >= VOTER_LIMIT || winner}
      className="vote-btn"
    >
      Party 1
    </button>
    <button
      onClick={() => handleVote(1)}
      disabled={totalVotes >= VOTER_LIMIT || winner}
      className="vote-btn"
    >
      Party 2
    </button>
    <button
      onClick={() => handleVote(2)}
      disabled={totalVotes >= VOTER_LIMIT || winner}
      className="vote-btn"
    >
      Party 3
    </button>
  </div>
)}
      </div>
      <div>
         {/* Message Display */}
      <p className="message">{message}</p>


{/* LED Indicators */}
<div className="led-section">
  <div className={`led ${ledStatus[0] ? "on" : ""}`}>
    <svg width="50" height="50" xmlns="http://www.w3.org/2000/svg">
      <circle cx="25" cy="25" r="20" fill={ledStatus[0] ? "green" : "gray"} />
    </svg>
    LED 1
  </div>
  <div className={`led ${ledStatus[1] ? "on" : ""}`}>
    <svg width="50" height="50" xmlns="http://www.w3.org/2000/svg">
      <circle cx="25" cy="25" r="20" fill={ledStatus[1] ? "green" : "gray"} />
    </svg>
    LED 2
  </div>
  <div className={`led ${ledStatus[2] ? "on" : ""}`}>
    <svg width="50" height="50" xmlns="http://www.w3.org/2000/svg">
      <circle cx="25" cy="25" r="20" fill={ledStatus[2] ? "green" : "gray"} />
    </svg>
    LED 3
  </div>
</div>


<div 
  className="results" 
  style={{
    background: 'linear-gradient(135deg,rgb(137, 110, 165),rgb(101, 250, 178))', 
    padding: '10px', 
    borderRadius: '16px', 
    width: '350px', 
    margin: '10px auto', 
    textAlign: 'center',
    color: '#fff',
    fontFamily: 'Arial, sans-serif',
    boxShadow: '0px 8px 16px rgba(0, 0, 0, 0.2)'
  }}
>
  <h3 style={{
    fontSize: '24px', 
    fontWeight: 'bold', 
    marginBottom: '15px'
  }}>
    Team Leader : Vote Counts:
  </h3>
  
  {/* Brijesh */}
  <div style={{
    background: 'rgba(255, 255, 255, 0.2)', 
    padding: '10px', 
    margin: '5px 0', 
    borderRadius: '12px', 
    boxShadow: '0px 4px 8px rgba(0, 0, 0, 0.1)',
    backdropFilter: 'blur(10px)',
    display: 'flex',
    alignItems: 'center'
  }}>
    <img 
      src="brijesh.jpg" 
      alt="Brijesh" 
      style={{
        width: '40px',
        height: '40px',
        borderRadius: '50%',
        marginRight: '10px'
      }}
    />
    <p style={{
      margin: '0', 
      fontSize: '18px', 
      fontWeight: '500'
    }}>
      Brijesh : {voteCounts[0]}
    </p>
  </div>
  
  {/* Deepika */}
  <div style={{
    background: 'rgba(255, 255, 255, 0.2)', 
    padding: '10px', 
    margin: '5px 0', 
    borderRadius: '12px', 
    boxShadow: '0px 4px 8px rgba(0, 0, 0, 0.1)',
    backdropFilter: 'blur(10px)',
    display: 'flex',
    alignItems: 'center'
  }}>
    <img 
      src="deepika.jpeg" 
      alt="Deepika" 
      style={{
        width: '40px',
        height: '40px',
        borderRadius: '50%',
        marginRight: '10px'
      }}
    />
    <p style={{
      margin: '0', 
      fontSize: '18px', 
      fontWeight: '500'
    }}>
      Deepika : {voteCounts[1]}
    </p>
  </div>
  
  {/* Ritik */}
  <div style={{
    background: 'rgba(255, 255, 255, 0.2)', 
    padding: '10px', 
    margin: '5px 0', 
    borderRadius: '12px', 
    boxShadow: '0px 4px 8px rgba(0, 0, 0, 0.1)',
    backdropFilter: 'blur(10px)',
    display: 'flex',
    alignItems: 'center'
  }}>
    <img 
      src="ritik.jpg" 
      alt="Ritik" 
      style={{
        width: '40px',
        height: '40px',
        borderRadius: '50%',
        marginRight: '10px'
      }}
    />
    <p style={{
      margin: '0', 
      fontSize: '18px', 
      fontWeight: '500'
    }}>
      Ritik : {voteCounts[2]}
    </p>
  </div>
</div>

 </div>
     <div className="rt">
      
{/* Leads */}
<div className="leads">
  <h3>Current Leads:</h3>
  <p>
    {Math.max(...voteCounts) > 0
      ? `Party ${
          voteCounts
            .map((count, index) =>
              count === Math.max(...voteCounts) ? index + 1 : null
            )
            .filter((party) => party !== null)
            .join(" and ")
        }`
      : "No votes yet"}
  </p>
</div>

{/* Winner */}
{winner && (
  <div className="winner">
    <h3>Winner:</h3>
    <p>{winner}</p>
  </div>
)}

{/* Reset Button */}
<button className="reset-button" onClick={resetSystem}>
  Reset System
</button>
     </div>
      <ToastContainer />
    </div></>
  );
}

export default App;
