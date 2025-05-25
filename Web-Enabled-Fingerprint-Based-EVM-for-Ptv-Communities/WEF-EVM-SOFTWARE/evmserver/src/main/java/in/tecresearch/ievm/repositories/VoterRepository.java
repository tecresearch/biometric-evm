package in.tecresearch.ievm.repositories;

import in.tecresearch.ievm.entities.Voter;
import org.springframework.data.mongodb.repository.MongoRepository;
import java.util.Optional;

public interface VoterRepository extends MongoRepository<Voter, String> {
    Optional<Voter> findByVoterId(String voterId);
    Optional<Voter> findByFingerprintData(String fingerprintData);
    boolean existsByFingerprintData(String fingerprintData);
}