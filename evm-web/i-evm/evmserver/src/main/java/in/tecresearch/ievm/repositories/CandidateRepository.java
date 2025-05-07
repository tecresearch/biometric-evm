package in.tecresearch.ievm.repositories;

import in.tecresearch.ievm.entities.Candidate;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;

import java.util.List;

public interface CandidateRepository extends MongoRepository<Candidate,String> {

    @Query("{ 'position' : ?0 }")
    List<Candidate> findByPosition(String position);
}
