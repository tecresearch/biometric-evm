package in.tecresearch.ievm.services.impl;

import in.tecresearch.ievm.entities.Voter;
import in.tecresearch.ievm.exceptions.DuplicateVoterException;
import in.tecresearch.ievm.exceptions.VoterNotFoundException;
import in.tecresearch.ievm.repositories.VoterRepository;
import in.tecresearch.ievm.services.VoterService;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

@Service
public class VoterServiceImpl implements VoterService {

    private final VoterRepository voterRepository;

    public VoterServiceImpl(VoterRepository voterRepository) {
        this.voterRepository = voterRepository;
    }

    @Override
    public Voter registerVoter(Voter voter) {
        if (voterRepository.existsByFingerprintData(voter.getFingerprintData())) {
            throw new DuplicateVoterException("Fingerprint already registered");
        }
        voter.setHasVoted(false);
        return voterRepository.save(voter);
    }

    @Override
    public Optional<Voter> getVoterById(String voterId) {
        return voterRepository.findByVoterId(voterId);
    }

    @Override
    public Optional<Voter> getVoterByFingerprint(String fingerprintData) {
        return voterRepository.findByFingerprintData(fingerprintData);
    }

    @Override
    public void markVoterAsVoted(String voterId) {
        Voter voter = voterRepository.findByVoterId(voterId)
                .orElseThrow(() -> new VoterNotFoundException("Voter not found"));
        
        if (voter.isHasVoted()) {
            throw new IllegalStateException("Voter has already cast their vote");
        }
        
        voter.setHasVoted(true);
        voterRepository.save(voter);
    }

    @Override
    public boolean verifyFingerprint(String fingerprintData) {
        return voterRepository.findByFingerprintData(fingerprintData).isPresent();
    }

    @Override
    public boolean verifyVoterAndFingerprint(String voterId, String fingerprintData) {
        return voterRepository.findByVoterId(voterId)
                .map(voter -> voter.getFingerprintData().equals(fingerprintData))
                .orElse(false);
    }

    @Override
    public List<Voter> getAllVoter() {
        return voterRepository.findAll();
    }
}