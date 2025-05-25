package in.tecresearch.ievm.services;

import in.tecresearch.ievm.entities.Voter;

import java.util.List;
import java.util.Optional;

public interface VoterService {
    Voter registerVoter(Voter voter);
    Optional<Voter> getVoterById(String voterId);
    Optional<Voter> getVoterByFingerprint(String fingerprintData);
    void markVoterAsVoted(String voterId);
    boolean verifyFingerprint(String fingerprintData);
    boolean verifyVoterAndFingerprint(String voterId, String fingerprintData);
    List<Voter> getAllVoter();
}